# This is a quick utility to check if a file is pure ASCII and if not, shows the lines and characters that are not ASCII

filename = 'tests/raven.txt'

with open(filename) as f:
    content = f.readlines()

    for line in content:
        if not line.isascii():
            print(line)
            for char in line:
                if not char.isascii():
                    print(char)
