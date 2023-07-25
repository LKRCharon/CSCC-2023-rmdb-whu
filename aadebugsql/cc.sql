create table concurrency_test (id int, name char(8), score float);
insert into concurrency_test values (1, 'xiaohong', 90.0);
insert into concurrency_test values (2, 'xiaoming', 95.0);
insert into concurrency_test values (3, 'zhanghua', 88.5); 

insert into concurrency_test values (4, 'zhanghua', 80.5); 
begin;

update concurrency_test set score = 100.0 where id = 2;

abort;

select * from concurrency_test where id = 2;

 

事务2:

begin;

select * from concurrency_test where id = 2;

commit;

操作序列：t1a t2a t1b t2b t1c t1d
2020-11-09 11:11:11