with open("input.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 bigint);\n');
    for id in range(1,1001):
        file.write("insert into d values(%d,'name',1,2,3);\n"%id);
    file.write('create index d(id);\n');
    file.write('select * from d where id = 1000;');

    