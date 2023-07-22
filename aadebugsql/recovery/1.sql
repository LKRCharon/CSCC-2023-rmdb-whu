create table t1 (id int, num int);
begin;
insert into t1 values(1, 1);
insert into t1 values(2, 2);
commit;
begin;
insert into t1 values(3, 3);
crash
