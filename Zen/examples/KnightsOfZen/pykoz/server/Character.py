import csv
reader = csv.reader(open("character.csv", "rb"))
for row in reader:
    print row

raw_input("press <enter>")
