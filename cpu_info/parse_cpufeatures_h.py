import sys
import re

PREFIX = '#define X86_FEATURE_'

features = []

for line in sys.stdin:
    if not line.startswith(PREFIX):
        continue
    line = line[len(PREFIX):].strip()
    matches = re.match(r'([^\s]+)\s*\(\s?(\d+)\*32\+\s?(\d+)\)\s*/\*\s*(.*)\s*\*/', line)
    if matches:
        identifier = matches.group(1)
        word = int(matches.group(2))
        index = int(matches.group(3))
        comment = matches.group(4)
        
        custom_name = re.match(r'("[^"]*")\s*(.*)', comment)
        if custom_name:
            identifier = custom_name.group(1)
            if identifier == '""':
                continue
            identifier = identifier.strip('"')
            comment = custom_name.group(2)
        
        identifier = identifier.lower()
        features.append(identifier)
    else:
        raise ValueError(f'"{line}" has unexpected format')

features.sort()

for feat in features:
    print(f'CPU_FEATURE_DEF({feat})')
