from sys import argv

ip = open(argv[1])
train_files = [open('%s.train+dev.%u' % (argv[1], i), 'w') 
               for i in range(10)]
test_files = [open('%s.test.%u' % (argv[1], i),'w') 
              for i in range(10)]

for i, line in enumerate(ip):
    for j in range(10):
        if i % 10 == j:
            test_files[j].write(line)
        else:
            train_files[j].write(line)
