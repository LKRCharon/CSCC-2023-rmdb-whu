with open("aadebugsql/multiple1.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 float);\n');
    file.write('create index d(id,test3);\n');
    file.write('create index d(test3,id);\n');
    file.write('create index d(test,id);\n');
    # for id in range(1,120000):
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
    file.write('select * from d where test=1;');

with open("aadebugsql/bnlj1.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 float);\n');
    for id in range(1,101):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
    file.write('select * from d;\n');
    file.write('select * from d;\n');
    file.write('select * from d;\n');

    