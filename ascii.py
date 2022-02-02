filename = 'tests/raven.txt'

with open(filename) as f:
    content = f.readlines()

    for line in content:
        if not line.isascii():
            print(line)
            for char in line:
                if not char.isascii():
                    print(char)
