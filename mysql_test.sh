#!/bin/sh

utest_num=0
function UTest()
{
let utest_num++

/usr/local/mysql/bin/mysql -uroot#12345467 -p'123456' -S /tmp/mysql.sock -e"
tee /tmp/temp.log
use test;
$1
notee
quit"

if [ $? != 0 ] 
then
    echo -e "\e[31mUTest $utest_num run failed!\e[0m"
    exit 1
fi

echo -e "\e[32mUTest $utest_num run success!\e[0m"

return 0
}


#create databases

[ -f "/tmp/temp.log" ] && rm -rf /tmp/temp.log

/usr/local/mysql/bin/mysql -uroot#12345467 -p'123456' -S /tmp/mysql.sock -e"
tee /tmp/temp.log

drop database if exists test;

create database test;

use test;

#有主键，有索引，有唯一性索引，并且唯一性索引和一般索引都进行了加密。
create table test1(id int(5), name varchar(100), tel varchar(50), addr varchar(100), age int(5), primary key(id), unique(tel), index idx_test1_name(name));
insert into test1 values(1, 'wangbiaoshuai', '18192751835', 'gaoxinqu', 27);
insert into test1 values(2, 'kongyue', '18706873980', 'shanyang', 23);
insert into test1 values(3, 'wangqiang', '18900091829', 'dazhailu', 24);


#无任何性质的索引
create table test2(id int(5), tel varchar(50), name varchar(100), addr varchar(100));
insert into test2 values(1, '18192751835', 'wangbiaoshuai', 'gaoxinqu');
insert into test2 values(2, '18706873980', 'kongyue', 'shanyang');
insert into test2 values(3, '19091941094', 'renzhicheng', 'zhongruanguoji');


#有联合主键索引，并且主键索引进行了加密
create table test3(id int(5), tel varchar(50), name varchar(100), addr varchar(100), primary key(tel, name));
insert into test3 values(1, '18192751835', 'wangbiaoshuai', 'gaoxinqu');
insert into test3 values(2, '18706873980', 'kongyue', 'shanyang');
insert into test3 values(3, '19091941094', 'renzhicheng', 'zhongruanguoji');
insert into test3 values(4, '13892053764', 'zhanyulan', 'ruanjianyuan');
insert into test3 values(5, '15129827542', 'huangying', 'jintai');

#有多列索引
create table test4(id int(5), tel varchar(50), name varchar(100), addr varchar(100), index idx_tel_name(tel, name), primary key(id));
insert into test4 values(1, '18192751835', 'wangbiaoshuai', 'gaoxinqu');
insert into test4 values(2, '18706873980', 'kongyue', 'shanyang');
insert into test4 values(3, '19091941094', 'renzhicheng', 'zhongruanguoji');
insert into test4 values(4, '18691082681', 'zhanglingling', 'hebeishijiazhuang');
insert into test4 values(5, '18628486660', 'zhangjunjie', 'hebeishijiazhuang');

notee
quit"


UTest "select * from test1;"
UTest "select name from test1 where tel='18706873980';"
UTest "select tel from test1 where name='wangbiaoshuai';"
UTest "select addr from test1 where id=3;"
UTest "select name from test1 where tel=(select tel from test1 where addr='dazhailu');"

UTest "select * from test2;"
UTest "select name from test2 where tel='18706873980';"
UTest "select tel from test2 where name='wangbiaoshuai';"
UTest "select addr from test2 where id=3;"
UTest "select name from test2 where tel=(select tel from test2 where addr='zhongruanguoji');"
UTest "select name from test1 where tel=(select tel from test2 where addr='shanyang');"

UTest "select * from test3;"
UTest "select name from test3 where tel='18706873980';"
UTest "select tel from test3 where name='wangbiaoshuai';"
UTest "select addr from test3 where id=3;"
UTest "select tel, name from test3 where addr='ruanjianyuan';"
UTest "select name from test3 where tel=(select tel from test1 where addr='gaoxinqu');"
UTest "select addr from test3 where name=(select name from test2 where tel=(select tel from test1 where addr='gaoxinqu'));"

UTest "select * from test4 order by id;"
UTest "select name from test4 where tel='18706873980';"
UTest "select tel from test4 where name='wangbiaoshuai';"
UTest "select addr from test4 where id=3;"
UTest "select tel, name from test4 where addr='hebeishijiazhuang';"
UTest "select name from test4 where tel=(select tel from test1 where addr='gaoxinqu');"
UTest "select addr from test4 where name=(select name from test3 where tel=(select tel from test1 where addr='gaoxinqu'));"

#总共会产生17个选择结果

