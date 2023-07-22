create table t1 (id int, num int);
insert into t1 values(1, 1);
insert into t1 values(2, 2);
insert into t1 values(3, 3);
create index t1(id);
begin;
update t1 set num=4 where id=2;
abort;
crash
select * from t1;
crash