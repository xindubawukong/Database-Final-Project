import csv


def get_all_lines(path):
    with open(path, 'r') as f:
        res = []
        lines = f.readlines()
        for line in lines:
            res.append(line.split('|'))
        return res


def main():
    insts = []
    insts.append('create database test;')
    insts.append('show databases;')
    insts.append('use test;')


    path = '../dataset_small_v2/part.tbl'
    lines = get_all_lines(path)
    info = [
        ('P_PARTKEY', 'INT'),
        ('P_NAME', 'VARCHAR(60)'),
        ('P_MFGR', 'VARCHAR(30)'),
        ('P_BRAND', 'VARCHAR(15)'),
        ('P_TYPE', 'VARCHAR(30)'),
        ('P_SIZE', 'INT'),
        ('P_CONTAINER', 'VARCHAR(15)'),
        ('P_RETAILPRICE', 'FLOAT'),
        ('P_COMMENT', 'VARCHAR(25)')
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
    

    path = '../dataset_small_v2/region.tbl'
    lines = get_all_lines(path)
    info = [
        ('R_REGIONKEY', 'INT'),
		('R_NAME', 'VARCHAR(30)'),
		('R_COMMENT', 'VARCHAR(155)'),
    ]
    inst = 'create table REGION('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)
    for line in lines:
        inst = 'insert into REGION values ('
        for i in range(len(info)):
            if i > 0:
                inst += ', '
            if info[i][1][:4] == 'CHAR' or info[i][1][:7] == 'VARCHAR':
                inst += '\'' + line[i] + '\''
            else:
                inst += line[i]
        inst += ');'
        insts.append(inst)
    

    path = '../dataset_small_v2/nation.tbl'
    lines = get_all_lines(path)
    info = [
        ('N_NATIONKEY', 'INT'),
		('N_NAME', 'VARCHAR(30)'),
		('N_REGIONKEY',	'INT'),
		('N_COMMENT', 'VARCHAR(155)'),
    ]
    inst = 'create table NATION('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)
    for line in lines:
        inst = 'insert into NATION values ('
        for i in range(len(info)):
            if i > 0:
                inst += ', '
            if info[i][1][:4] == 'CHAR' or info[i][1][:7] == 'VARCHAR':
                inst += '\'' + line[i] + '\''
            else:
                inst += line[i]
        inst += ');'
        insts.append(inst)
    

    path = '../dataset_small_v2/supplier.tbl'
    lines = get_all_lines(path)
    info = [
        ('S_SUPPKEY', 'INT'),
		('S_NAME', 'VARCHAR(30)'),
		('S_ADDRESS', 'VARCHAR(45)'),
		('S_NATIONKEY',	'INT'),
		('S_PHONE', 'VARCHAR(20)'),
		('S_ACCTBAL', 'FLOAT'),
		('S_COMMENT', 'VARCHAR(105)')
    ]
    inst = 'create table SUPPLIER('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)
    for line in lines:
        inst = 'insert into SUPPLIER values ('
        for i in range(len(info)):
            if i > 0:
                inst += ', '
            if info[i][1][:4] == 'CHAR' or info[i][1][:7] == 'VARCHAR':
                inst += '\'' + line[i] + '\''
            else:
                inst += line[i]
        inst += ');'
        insts.append(inst)
        
    
    path = '../dataset_small_v2/customer.tbl'
    lines = get_all_lines(path)
    info = [
        ('C_CUSTKEY', 'INT'),
		('C_NAME', 'VARCHAR(30)'),
		('C_ADDRESS', 'VARCHAR(45)'),
		('C_NATIONKEY', 'INT'),
		('C_PHONE', 'VARCHAR(20)'),
		('C_ACCTBAL', 'FLOAT'),
		('C_MKTSEGMENT', 'VARCHAR(15)'),
		('C_COMMENT', 'VARCHAR(120)')
    ]
    inst = 'create table CUSTOMER('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)
    for line in lines:
        inst = 'insert into CUSTOMER values ('
        for i in range(len(info)):
            if i > 0:
                inst += ', '
            if info[i][1][:4] == 'CHAR' or info[i][1][:7] == 'VARCHAR':
                inst += '\'' + line[i] + '\''
            else:
                inst += line[i]
        inst += ');'
        insts.append(inst)

    
    path = '../dataset_small_v2/partsupp.tbl'
    lines = get_all_lines(path)
    info = [
        ('PS_PARTKEY', 'INT'),
		('PS_SUPPKEY', 'INT'),
		('PS_AVAILQTY', 'INT'),
		('PS_SUPPLYCOST', 'FLOAT'),
		('PS_COMMENT', 'VARCHAR(205)')
    ]
    inst = 'create table PARTSUPP('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)
    for line in lines:
        inst = 'insert into PARTSUPP values ('
        for i in range(len(info)):
            if i > 0:
                inst += ', '
            if info[i][1][:4] == 'CHAR' or info[i][1][:7] == 'VARCHAR':
                inst += '\'' + line[i] + '\''
            else:
                inst += line[i]
        inst += ');'
        insts.append(inst)
    

    path = '../dataset_small_v2/orders.tbl'
    lines = get_all_lines(path)
    info = [
        ('O_ORDERKEY', 'INT'),
		('O_CUSTKEY', 'INT'),
		('O_ORDERSTATUS', 'VARCHAR(5)'),
		('O_TOTALPRICE', 'FLOAT'),
		('O_ORDERDATE', 'VARCHAR(20)'),
		('O_ORDERPRIORITY', 'VARCHAR(20)'),
		('O_CLERK',	'VARCHAR(20)'),
		('O_SHIPPRIORITY', 'INT'),
		('O_COMMENT', 'VARCHAR(85)')
    ]
    inst = 'create table ORDERS('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)
    for line in lines:
        inst = 'insert into ORDERS values ('
        for i in range(len(info)):
            if i > 0:
                inst += ', '
            if info[i][1][:4] == 'CHAR' or info[i][1][:7] == 'VARCHAR':
                inst += '\'' + line[i] + '\''
            else:
                inst += line[i]
        inst += ');'
        insts.append(inst)
    

    path = '../dataset_small_v2/lineitem.tbl'
    lines = get_all_lines(path)
    info = [
        ('L_ORDERKEY', 'INT'),
		('L_PARTKEY', 'INT'),
		('L_SUPPKEY', 'INT'),
		('L_LINENUMBER', 'INT'),
		('L_QUANTITY', 'FLOAT'),
		('L_EXTENDEDPRICE', 'FLOAT'),
		('L_DISCOUNT', 'FLOAT'),
		('L_TAX', 'FLOAT'),
		('L_RETURNFLAG', 'VARCHAR(5)'),
		('L_LINESTATUS', 'VARCHAR(5)'),
		('L_SHIPDATE', 'VARCHAR(20)'),
		('L_COMMITDATE', 'VARCHAR(20)'),
		('L_RECEIPTDATE', 'VARCHAR(20)'),
		('L_SHIPINSTRUCT', 'VARCHAR(30)'),
		('L_SHIPMODE', 'VARCHAR(15)'),
		('L_COMMENT', 'VARCHAR(50)'),
    ]
    inst = 'create table LINEITEM('
    for i in range(len(info)):
        inst += '\n    '
        for t in info[i]:
            inst += t + ' '
        if i < len(info) - 1:
            inst += ','
    inst += ');'
    insts.append(inst)
    for line in lines:
        inst = 'insert into LINEITEM values ('
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