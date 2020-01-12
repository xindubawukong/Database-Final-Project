import csv


def get_all_lines(path):
    with open(path,'r') as file:
        reader = csv.reader(file)
        lines = []
        for line in reader:
            lines.append(line)
        return lines


def main():
    insts = []
    insts.append('create database test;')
    insts.append('show databases;')
    insts.append('use test;')


    path = '../dataset_small_orderdb/part.csv'
    lines = get_all_lines(path)
    info = [
        ('P_PARTKEY', 'INT'),
        ('P_NAME', 'VARCHAR(55)'),
        ('P_MFGR', 'VARCHAR(25)'),
        ('P_BRAND', 'VARCHAR(10)'),
        ('P_TYPE', 'VARCHAR(25)'),
        ('P_SIZE', 'INT'),
        ('P_CONTAINER', 'VARCHAR(10)'),
        ('P_RETAILPRICE', 'FLOAT'),
        ('P_COMMENT', 'VARCHAR(23)')
    ]
    inst = 'create table PART('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)

    for line in lines:
        inst = 'insert into PART values ('
        for i in range(len(info)):
            if i > 0:
                inst += ', '
            if info[i][1][:4] == 'CHAR' or info[i][1][:7] == 'VARCHAR':
                inst += '\'' + line[i] + '\''
            else:
                inst += line[i]
        inst += ');'
        insts.append(inst)
        

    with open('../generated.txt', 'w') as f:
        for inst in insts:
            f.write(inst + '\n')


if __name__ == '__main__':
    main()