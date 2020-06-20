import jieba

charlen = 17993

filename = 'article1_compressed.bin'
file_text = open(filename, 'r')
text = file_text.read()
text = text.split(',')

filename = 'article1.txt'
file_text = open(filename, 'r')
lipsum = file_text.read()
lipsum = jieba.cut(lipsum, cut_all = False)
lipsum = ','.join(lipsum)
lipsum = lipsum.split(',')
    
for x in range(len(text)):
    text[x] = text[x].split(':')

length = 0
for i in range(len(lipsum)):
    for j in range(len(text)):
        # print(lipsum[i]+ '\'')
        # print(text[j][0][2:])
        if lipsum[i]+ '\'' == text[j][0][2:]:
            length = length + len(text[j][1]) - 2
            if j == len(text) - 1:
                length = length - 1

print("character num: " + str(charlen))
print("encoded bit num: " + str(length))
print("ratio: " + str(length / charlen * 8))