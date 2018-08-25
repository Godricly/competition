label = open('sample_geek_result.csv')
labellines = label.readlines()

result = open('result.csv')
resultlines = result.readlines()

gt = {}
for line in labellines:
    gt[line] = True

counter = 0
for line in resultlines:
    if line in gt:
        counter+=1
    
print('accuracy:', 1.0*counter/len(labellines))
