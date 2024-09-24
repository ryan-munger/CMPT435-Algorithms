with Ada.Text_IO; use Ada.Text_IO;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with Ada.Strings.Unbounded.Text_IO;
with Ada.Containers.Indefinite_Vectors;
with Ada.Characters.Handling;

procedure assignment1 is

   -- Declare the Node record for Stack and Queue
   type Node; 
   type Node_Ptr is access Node; 

   type Node is
      record
         Data : Character;
         Next : Node_Ptr;
      end record;

   -- Declare stack
   type Stack is
      record
         Top : Node_Ptr := null;
      end record;

   -- Declare queue
   type Queue is
      record
         Front : Node_Ptr := null;
         Rear  : Node_Ptr := null;
      end record;

   -- prodedures are void
   procedure Push(S : in out Stack; Value : Character) is
      New_Node : Node_Ptr := new Node;
   begin
      New_Node.Data := Value;
      New_Node.Next := S.Top;
      S.Top := New_Node;
   end Push;

   -- functions return a value
   function Pop(S : in out Stack) return Character is
      Temp_Node : Node_Ptr;
      Value : Character;
   begin
      if S.Top = null then
         raise Program_Error;  -- Stack underflow?
      else
         Temp_Node := S.Top;
         Value := Temp_Node.Data;
         S.Top := S.Top.Next;
         return Value;
      end if;
   end Pop;

   procedure Enqueue(Q : in out Queue; Value : Character) is
      New_Node : Node_Ptr := new Node;
   begin
      New_Node.Data := Value;
      New_Node.Next := null;
      
      if Q.Rear = null then
         -- Queue is empty, so Front and Rear point to the new node
         Q.Front := New_Node;
         Q.Rear := New_Node;
      else
         -- Append to the rear of the queue
         Q.Rear.Next := New_Node;
         Q.Rear := New_Node;
      end if;
   end Enqueue;

   function Dequeue(Q : in out Queue) return Character is
      Temp_Node : Node_Ptr;
      Value : Character;
   begin
      if Q.Front = null then
         raise Program_Error;  -- Queue underflow?
      else
         Temp_Node := Q.Front;
         Value := Temp_Node.Data;
         Q.Front := Q.Front.Next;

         --  if front null its empty so rear null
         if Q.Front = null then
            Q.Rear := null;
         end if;

         return Value;
      end if;
   end Dequeue;

   function Is_Palindrome(Input : String) return Boolean is
      S : Stack;
      Q : Queue;
      Len : Integer := Input'Length;
   begin
      -- Load the string into both the stack and the queue
      -- Ingore case and space
      for I in 1 .. Len loop
         if Input(I) /= ' ' then
            Push(S, Ada.Characters.Handling.To_Lower(Input(I)));
            Enqueue(Q, Ada.Characters.Handling.To_Lower(Input(I)));
         end if;
      end loop;

      -- Pop, dequeue, compare
      for I in 1 .. Len loop
         if Input(I) /= ' ' then
            if Pop(S) /= Dequeue(Q) then
               return False;
            end if;
         end if;
      end loop;

      return True;
   end Is_Palindrome;

   -- Procedure to load phrases from a file
   procedure Check_Magicitems(File_Name : String) is
      File : File_Type;
      Line : String (1 .. 100);  -- assuming max line length is 100 characters
      Length : Natural;
   begin
      Open (File, In_File, File_Name);
      while not End_Of_File (File) loop
         Get_Line (File, Line, Length);
         
         if Is_Palindrome(Line (1 .. Length)) then
            Put_Line(Line (1 .. Length) & " is a palindrome.");
         end if;
         
      end loop;
      Close (File);
   end Check_Magicitems;

   N1: Node_Ptr := new Node;
   N2: Node_Ptr := new Node;
begin
   N1.Data := 'a';
   N2.Data := 'b';
   N1.Next := N2;
   Put_Line("Node1 Value: " & N1.Data);
   Put_Line("Node2 Value: " & N2.Data);
   if N1.Next = N2 then
      Put_Line("Node1 Points to Node2!");
   else
      Put_Line("Node1 Does not Point to Node2!");
   end if;

   Put_Line("");

   Check_Magicitems("../../magicitems.txt");

end assignment1;

-- In sum: walrus operator is pretty cool. Prodedures and functions are different. Ada is not easy (yet).