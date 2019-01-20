{
int x = 5;
int z = 7;
bool b = true;
bool b2 = false;

if(2==2) print(1); else print(2); // debe dar 1
if(x==2) print(3); else print(4); // debe dar 4
if(x!=2) print(5); else print(6); // debe dar 5
if(x==z) print(7); else print(8); // debe dar 8
if(b==b2) print(9); else print(10); // debe dar 10
if(b!=b2) print(11); else print(12); // debe dar 11
if(b==!b2) print(13); else print(14); // debe dar 13
if(!(b==b2)) print(15); else print(16); // debe dar 15
//if(b==x) print(1); else print(0); // debe dar error
}
