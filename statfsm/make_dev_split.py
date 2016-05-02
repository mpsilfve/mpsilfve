from sys import stdin, argv

train_file = open(argv[1].replace('+dev',''), 'w')
dev_file = open(argv[1].replace('train+',''), 'w')

i = 0

for line in open(argv[1]):
    if i % 10 == 0:
        dev_file.write(line)
    else:
        train_file.write(line)

    if line == '\n':
        i += 1
