{$mode objfpc} // directive to be used for defining classes
{$m+}		   // directive to be used for using constructor

program Assignment1;
type
   Node = class
   public
    val: char;
    next: ^Node;
    constructor create(ch: char);
  
end;

constructor Node.create(ch: char);
begin
   val := ch;
end;

var
   node1: Node;
   node2: Node;

begin
   node1 := Node.create('a');
   writeln('Node value: ' + node1.val);
   node2 := Node.create('b');
   node1.next := @node2;
   if (node1.next = @node2) then
      writeln('Node1 Points to Node 2');
end.