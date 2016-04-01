task 1
The function first deals with terminal cases of "and", "or", "xor".
If it is a terminal case, return the corresponding pointer of the BDD. 

If it is not a terminal case, we start to check whether the variable of the two BDDs are the same.
If the two BDD nodes contains the same variable, 
we calculate result->neg_cf = BDD1->neg_cf op BDD2->neg_cf and we also calculate result->pos_cf = BDD1->pos_cf op BDD2->pos_cf by using the apply function recursively until we meet the terminal cases.
And then, we check whether it is in the unique table. If it isn't in the unique talbe, we put the result into the unique table, and return the pointer of the result. If it is in the unique table, we just return the pointer of the existing node in the unique table.

If the two BDD nodes do not contain the same variable.
If the variable of BDD1 is smaller than BDD2, 
we calculate result->neg_cf = BDD1->neg_cf op BDD2 and we also calculate result->pos_cf = BDD1->pos_cf op BDD2 by using the apply function recursively until we meet the terminal cases.
If the variable of BDD2 is smaller than BDD1, 
we calculate result->neg_cf = BDD1 op BDD2->neg_cf and we also calculate result->pos_cf = BDD1 op BDD2->pos_cf by using the apply function recursively until we meet the terminal cases.

Then, we check whether it is in the unique table. If it isn't in the unique talbe, we put the result into the unique table, and return the pointer of the result. If it is in the unique table, we just return the pointer of the existing node in the unique table.

*********************************************

task 2
1. the function of negative_cofactor
The parameter of this function contains two variables: the BDD pointer BDD_PTR np and the variable var.
In this task, we firstly check whether the nodes contain the input variable "var" from the root of the BDD np to the bottom of this BDD.
If the node doesn't contains the variable, we keep the node unchanged.
If the node contains the variable, we ignore the positive cofactor of the node and remain the negative cofactor part.
Since the current node only connects with the negative cofactor part, the node itself can be ignored. 
So we conntect its negative cofactor to its former nodes and put it into the unique table.

2. the function of negative_cofactor
The parameter of this function contains two variables: the BDD pointer BDD_PTR np and the variable var.
In this task, we firstly check whether the nodes contain the input variable "var" from the root of the BDD np to the bottom of this BDD.
If the node doesn't contains the variable, we keep the node unchanged.
If the node contains the variable, we ignore the negative cofactor of the node and remain the positive cofactor part.
Since the current node only connects with the positive cofactor part, the node itself can be ignored. 
So we conntect its positive cofactor to its former nodes and put it into the unique table.

3. the function of Boolean Difference
When we calculate the Boolean difference, we first call the function of negative_cofactor() and the function of positive_cofactor() to calculate the negative cofactor and the positive cofactor of np with respect to variable var.
Then we call the apply function to compute the negative cofactor XOR positive the cofactor. 
Finally, we return the pointer of the result.

*********************************************

task 3
Firstly, I write a function called "traverse_and_collect_var()" to store all the variables in the BDD in an array. This function traverses all the nodes of the BDD and pushes the newly found variable into this array.
This function returns the number of the all variables in this BDD.
Once the function collects all the variables of a BDD, I can check each variable and compute the Boolean difference of the BDD with respect to this variable. 
After all the Boolean difference are finished computing, the probability of each nodes are calculated. 
I set an array of a structure named output_result to store each variable and its influence. 
Then I use bubble sorting to change the output order of these nodes.
Finally, The function use the output stream to display influence of each node.

*********************************************

Bonus Question

test case 1: 
a&c&d|b&c

c,0.625
b,0.375
d,0.125
a,0.125

result:
a 10
 |__b 9
 |  |__c 3
 |  |  |__1
 |  |  |__0
 |  |__c 5
 |     |__d 4
 |     |  |__1
 |     |  |__0
 |     |__0
 |__b 8
    |__c 3
    |  |__1
    |  |__0
    |__0

the number of nodes is 6.

if we change the order of literal according the influence of their value: 
c&a&d|b&a

a,0.625
b,0.375
d,0.125
c,0.125

result:
a 11
 |__b 10
 |  |__1
 |  |__c 6
 |     |__d 4
 |     |  |__1
 |     |  |__0
 |     |__0
 |__0

the number of nodes is 4.

*********************************************

test case 2:
(c&!b)|(!a&c)|(!c&d)

d,0.5
c,0.5
b,0.25
a,0.25

result:
a 15
 |__b 14
 |  |__c 11
 |  |  |__0
 |  |  |__d 10
 |  |     |__1
 |  |     |__0
 |  |__c 12
 |     |__1
 |     |__d 10
 |        |__1
 |        |__0
 |__c 12
    |__1
    |__d 10
       |__1
       |__0

the number of nodes is 5.


if we change the order of literal according the influence of their value: 
(a&!b)|(!c&a)|(!a&d)

b,0.5
a,0.5
c,0.25
d,0.25

result:
a 14
 |__c 13
 |  |__d 4
 |  |  |__0
 |  |  |__1
 |  |__1
 |__b 10
    |__1
    |__0

the number of nodes is 4.

*********************************************

test case 3:
(a|b|d)&(d&!a|b)

influence:
b,0.75
d,0.25
a,0.25

result:
a 9
 |__b 3
 |  |__1
 |  |__0
 |__b 5
    |__1
    |__d 4
       |__1
       |__0
the number of nodes is 4.

if we change the order of literal according the influence of their value: 
(a|d|b)&(b&!a|d)

influence:
d,0.75
b,0.25
a,0.25

result:
a 9
 |__d 4
 |  |__1
 |  |__0
 |__b 5
    |__1
    |__d 4
       |__1
       |__0

the number of nodes is 3.


if we change the order of literal according the influence of their value: 
(d|a|b)&(b&!d|a)

influence:
a,0.75
b,0.25
d,0.25

result:
a 10
 |__1
 |__b 9
    |__d 8
    |  |__0
    |  |__1
    |__0
the number of nodes is 3.

*********************************************

test case 4:
a&!b&!c|b&!c&d&e|!b&c&d|e&d

influence:
d,0.5625
e,0.3125
b,0.3125
c,0.1875
a,0.1875

result:
a 20
 |__b 19
 |  |__d 11
 |  |  |__e 10
 |  |  |  |__1
 |  |  |  |__0
 |  |  |__0
 |  |__c 18
 |     |__d 9
 |     |  |__1
 |     |  |__0
 |     |__1
 |__b 17
    |__d 11
    |  |__e 10
    |  |  |__1
    |  |  |__0
    |  |__0
    |__c 16
       |__d 9
       |  |__1
       |  |__0
       |__d 11
          |__e 10
          |  |__1
          |  |__0
          |__0

the number of nodes is 8.

if we change the order of literal according the influence of their value: 

a,0.5625
c,0.3125
b,0.3125
d,0.1875
e,0.1875

result:
a 26
 |__b 25
 |  |__c 11
 |  |  |__1
 |  |  |__0
 |  |__c 24
 |     |__1
 |     |__d 23
 |        |__1
 |        |__e 5
 |           |__1
 |           |__0
 |__b 9
    |__0
    |__d 8
       |__e 6
       |  |__0
       |  |__1
       |__0

the number of nodes is 9.

From testcase 1 and testcase 4, we can conclude that the size of BDDs cannot be reduced by reordering the variables according to their influence. 
In case 1, when we reordering the variables according to their influence, the size of ROBDD becomes smaller. 
However, in testcase 4, when we reordering the variables according to their influence, the size of ROBDD becomes larger.
So from the experiment, we can draw a conclusion that ordering by influence cannot help to reduce the size of a ROBDD.

The influence(f,xi) means how much a variable xi can influece on the value of the whole function f. 
If xi=1 and xi=0, the Boolean Difference always be 0, the function of f(x1,x2,...xi-1, 0, xi+1,....,xn) and f(x1,x2,...xi-1, 1, xi+1,....,xn) are the same. 
This situation means that no matter what value xi will be, the value of the function f will not change. So xi has no little influence on f.
And if the influence of xi is close to 1, it means that if the value of xi changes, it has a huge possibility that the value of f will change.

However, the influence doesn't related to the size of ROBDDs directly. Because even if xi has a huge influence on the value of f and we place it on the root of the ROBDD, if we place the cofactor of xi with a node which contains a variable that is far away from xi, the ROBDD may not have a smaller size. (e.g. the example2 given in page 31, lecture 7, each node has the same influence, but the different order of the variable will result in different ROBDD size)

I think the size of ROBDD might be smaller when the each pair of the adjacent nodes have great influence on each other.
