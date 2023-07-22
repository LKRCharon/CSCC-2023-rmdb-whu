with open("aadebugsql/index/txn1.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 float);\n');
    file.write('create index d(id);\n');
    file.write('begin;\n');
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
    file.write(f"delete from d where id>50 and id<100;\n");
    file.write('commit;\n');
    file.write('select * from d where id>0;');

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
    file.write('create table t1 (id int, name char(16),test bigint,test2 bigint);\n');
    file.write('create table t2 (t_id int, t_name char(16),t_test bigint,t_test2 bigint,t_test3 float);\n');
    for id in range(1,1001):
        file.write(f"insert into t1 values({id},'name',1,2);\n");
    for id in range(1,1001):
        file.write(f"insert into t2 values({id},'name',1,2,{id/1.7:.6f});\n");
    # file.write('select * from t1,t2 where t2.t_id>t1.id and t1.id>990 order by t1.id;\n');
    file.write('select * from t1,t2 where t2.t_id>t1.id and t1.id>999 order by t1.id;\n');

with open("aadebugsql/txn1.sql", "w") as file:
    file.write('create table d (id int, name char(16),time datetime,test2 bigint,test3 float);\n');
    file.write('create index d(time);\n');
    file.write('create index d(id);\n');
    
    # 插10条 abort
    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"insert into d values({id},'name','2023-07-0{id} 18:42:00',2,{id/1.7:.6f});\n");
    file.write('abort;\n');
    # file.write('select * from d;\n');
    # file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"insert into d values({id},'name','2023-07-0{id} 18:42:00',2,{id/1.7:.6f});\n");
    file.write('commit;\n');
    # file.write('select * from d;\n');
    # file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"update d set time='2023-08-0{id} 18:42:00' where time='2023-07-0{id} 18:42:00';\n");
    file.write('abort;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"update d set time='2023-08-0{id} 18:42:00' where time='2023-07-0{id} 18:42:00';\n");
    file.write('commit;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"delete from d where time='2023-08-0{id} 18:42:00';\n");
    file.write('abort;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"delete from d where time='2023-08-0{id} 18:42:00';\n");
    file.write('commit;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

with open("aadebugsql/txn1.sql", "w") as file:
    file.write('create table d (id int, name char(16),time datetime,test2 bigint,test3 float);\n');
    file.write('create index d(time);\n');
    file.write('create index d(id);\n');
    
    # 插10条 abort
    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"insert into d values({id},'name','2023-07-0{id} 18:42:00',2,{id/1.7:.6f});\n");
    file.write('abort;\n');
    # file.write('select * from d;\n');
    # file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"insert into d values({id},'name','2023-07-0{id} 18:42:00',2,{id/1.7:.6f});\n");
    file.write('commit;\n');
    # file.write('select * from d;\n');
    # file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"update d set time='2023-08-0{id} 18:42:00' where time='2023-07-0{id} 18:42:00';\n");
    file.write('abort;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"update d set time='2023-08-0{id} 18:42:00' where time='2023-07-0{id} 18:42:00';\n");
    file.write('commit;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"delete from d where time='2023-08-0{id} 18:42:00';\n");
    file.write('abort;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

    file.write('begin;\n');
    for id in range(1,11):
        file.write(f"delete from d where time='2023-08-0{id} 18:42:00';\n");
    file.write('commit;\n');
    file.write('select * from d;\n');
    file.write('select * from d where id>0;\n');

with open("aadebugsql/txn2_abort.sql", "w") as file:
    file.write('create table txn2 (id int,f float);\n');
    for id in range(1,4):
        file.write(f"insert into txn2 values({id},{id/1.7:.6f});\n");

    file.write('begin;\n');
    file.write('select * from txn2;\n');
    file.write(f"insert into txn2 values(4,100.0);\n");
    file.write('select * from txn2;\n');
    file.write(f"delete from txn2 where id>2;\n");
    file.write('select * from txn2;\n');
    file.write('abort;\n');
    file.write('select * from txn2;\n');



with open("aadebugsql/recovery/recovery.sql", "w") as file:
    file.write('create table d (id int, name char(30),test2 bigint,test3 float);\n');
    file.write('begin;\n');
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',2,{id/1.7:.6f});\n");

    for id in range(1,101):
        file.write(f"update d set test2={id} where id={id};\n");

    file.write(f"delete from d where id>50 and id<100;\n");
    file.write('commit;\n');
    file.write('begin;\n');
    for id in range(101,500):
        file.write(f"update d set test2={id} where id={id};\n");
    file.write(f"delete from d where id>101 and id<250;\n");
    file.write('abort;\n');
    file.write('crash\n');

with open("aadebugsql/recovery/single_thread_index.sql", "w") as file:
    file.write('create table d (id int, name char(30),test2 bigint,test3 float);\n');
    file.write('create index d(id);\n');
    file.write('begin;\n');
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',2,{id/1.7:.6f});\n");

    for id in range(1,101):
        file.write(f"update d set test2={id} where id={id};\n");

    file.write(f"delete from d where id>50 and id<100;\n");
    file.write('commit;\n');
    file.write('begin;\n');
    for id in range(101,500):
        file.write(f"update d set test2={id} where id={id};\n");
    file.write(f"delete from d where id>101 and id<250;\n");
    file.write('abort;\n');
    file.write('crash\n');
with open("aadebugsql/recovery/single_thread_index_update.sql", "w") as file:
    file.write('create table d (id int, name char(30),test2 bigint,test3 float);\n');
    file.write('create index d(id);\n');
    file.write('begin;\n');
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',2,{id/1.7:.6f});\n");

    for id in range(1,101):
        file.write(f"update d set id={id+1000} where id={id};\n");
    file.write('commit;\n');
    file.write('begin;\n');
    for id in range(1,101):
        file.write(f"update d set id={id} where id={id+1000};\n");
    file.write(f"delete from d where id>101 and id<250;\n");
    file.write('abort;\n');
    file.write('crash\n');

# TODO: Abort 回滚失败 07-21 19：00
with open("aadebugsql/single_thread_recovery/insert_test.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 float);\n');
    file.write('begin;\n');
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
    file.write('commit;\n');
    file.write('select * from d where id>0;\n');
    file.write('begin;\n');
    for id in range(1501,2001):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
    file.write('select * from d where id>0;\n');
    # file.write('abort;\n');
    # file.write('select * from d where id>0;\n');
    file.write('crash\n');

# TODO: Abort 回滚失败 07-21 19：00  delete->commit update delete abort
with open("aadebugsql/single_thread_recovery/update_delete_test.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 float);\n');
    file.write('begin;\n');
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
    file.write(f"delete from d where id<=50;\n");
    file.write('commit;\n');
    file.write('begin;\n');
    file.write(f"update d set test2={id} where id>=50 and id<101;\n");
    file.write("delete from d where id>=50 and id<101;\n");
    file.write('abort;\n');
    

with open("aadebugsql/single_thread_recovery/insert_join_test.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 float);\n');
    file.write('create table t (id int, name char(16));\n');
    file.write('begin;\n');
    for id in range(1,1001):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
        file.write(f"insert into t values({id},'name{id}');\n");
    file.write('commit;\n');
    file.write('select * from d,t;\n')

with open("aadebugsql/single_thread_recovery/big_data_test.sql", "w") as file:
    file.write('create table d (id int, name char(16),test bigint,test2 bigint,test3 float);\n');
    file.write('create table t (id int, name char(16));\n');
    file.write('begin;\n');
    for id in range(1,20001):
        file.write(f"insert into d values({id},'name',1,2,{id/1.7:.6f});\n");
        file.write(f"insert into t values({id},'name{id}');\n");
    file.write('commit;\n');
    file.write('begin;\n');
    file.write('select * from d where id>0;\n');
    # file.write(f"update d set test2={id} where id<50001;\n");

    