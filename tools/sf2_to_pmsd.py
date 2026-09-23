import os
import struct
import argparse
from sf2utils.sf2parse import Sf2File

PMSD_MAGIC = b'PMSD'
PMSD_VERSION = 1
PMSD_METHOD_RESAMPLER = 0

def midi_to_hz(midiNote):
    return 440.0 * (2.0 ** ((midiNote - 69.0) / 12.0))

def extract_smpl_chunk(sf2_path):
    """
    Parcourt manuellement la structure RIFF du fichier SF2
    pour extraire le gros bloc de données PCM (le chunk 'smpl').
    """
    with open(sf2_path, 'rb') as f:
        riff_magic = f.read(4)
        if riff_magic != b'RIFF':
            return b""
        
        f.read(8)
        
        while True:
            chunk_header = f.read(8)
            if len(chunk_header) < 8:
                break
            
            chunk_id, chunk_size = struct.unpack('<4sI', chunk_header)
            
            if chunk_id == b'LIST':
                list_type = f.read(4)
                if list_type == b'sdta':
                    bytes_left = chunk_size - 4
                    while bytes_left > 0:
                        sub_header = f.read(8)
                        sub_id, sub_size = struct.unpack('<4sI', sub_header)
                        
                        if sub_id == b'smpl':
                            return f.read(sub_size)
                        else:
                            # Sauter ce sous-chunk
                            pad = sub_size % 2
                            f.seek(sub_size + pad, 1)
                        
                        bytes_left -= (8 + sub_size + pad)
                else:
                    # Ce n'est pas la bonne liste, on la saute
                    pad = chunk_size % 2
                    f.seek(chunk_size - 4 + pad, 1)
            else:
                # Chunk non-LIST, on le saute
                pad = chunk_size % 2
                f.seek(chunk_size + pad, 1)
                
    return b""

def extract_sf2(sf2Path, outputDir):
    if not os.path.exists(outputDir):
        os.makedirs(outputDir)

    global_smpl_data = extract_smpl_chunk(sf2Path)
    
    if not global_smpl_data:
        print(f"Erreur : Impossible de trouver les données audio dans {sf2Path}")
        return

    with open(sf2Path, 'rb') as sf2File:
        sf2 = Sf2File(sf2File)
        print(f"Extract .sf2 file : {sf2Path}")
        
        for sample in sf2.samples:
            
            if not sample.name or sample.name == 'EOS':
                continue
            
            start_byte = sample.start * 2
            end_byte = sample.end * 2
            
            rawPcm = global_smpl_data[start_byte:end_byte]

            if not rawPcm:
                continue

            sampleCount = len(rawPcm) // 2 
            baseFreq = midi_to_hz(sample.original_pitch)

            loopStart = sample.start_loop - sample.start
            loopEnd = sample.end_loop - sample.start
            if loopStart < 0 or loopEnd < 0 or loopStart >= sampleCount:
                loopStart = 0
                loopEnd = 0
                loopEnabled = 0
            else:
                loopEnabled = 1 if (loopEnd > loopStart + 8) else 0

            headerFormat = '<4s I I H I I H'
            headerSize = struct.calcsize(headerFormat)
            
            metaFormat = '<f B I I'
            metaSize = struct.calcsize(metaFormat)

            dataOffset = headerSize + metaSize
            
            binHeader = struct.pack(
                headerFormat, 
                PMSD_MAGIC, 
                PMSD_VERSION, 
                dataOffset, 
                1, 
                sample.sample_rate, 
                sampleCount, 
                PMSD_METHOD_RESAMPLER
            )
            binMeta = struct.pack(metaFormat, baseFreq, loopEnabled, loopStart, loopEnd)

            safeName = "".join(c for c in sample.name if c.isalnum() or c in " _-")
            outFile = os.path.join(outputDir, f"{safeName}.pmsd")
            
            with open(outFile, 'wb') as f:
                f.write(binHeader)
                f.write(binMeta)
                f.write(rawPcm)
                
            print(f"[+] <PMSD:{outFile} ({baseFreq:.1f}Hz, Loop: {bool(loopEnabled)})>")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Extract all sample data of a .sf2 file and convert it to .pmsd file")
    parser.add_argument("sf2File", help="path of the .sf2 file")
    parser.add_argument("outputDir", help="Output directories of .pmsd")
    args = parser.parse_args()
    
    extract_sf2(args.sf2File, args.outputDir)