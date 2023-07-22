create table t1 (id int, num int);
insert into t1 values(1, 1);
insert into t1 values(2, 2);
insert into t1 values(3, 3);
create index t1(id);
begin;
insert into t1 values(3, 3);
insert into t1 values(4, 4);
select * from t1;
crash

