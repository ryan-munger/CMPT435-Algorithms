with Ada.Text_IO; use Ada.Text_IO;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with Ada.Strings.Fixed; use Ada.Strings.Fixed;
with Ada.Containers.Vectors;
with Ada.Strings.Maps.Constants;  
with Ada.IO_Exceptions;
with Ada.Numerics.Discrete_Random;

procedure Assignment2 is

   -- vector definition for dynamically allocated arrays of unbounded (dynamic) strings
    package Vectors is new Ada.Containers.Vectors (Index_Type => Positive, Element_Type => Unbounded_String);
    type Vector is tagged record
        Elements : Vectors.Vector;
    end record;
    use Vectors;

    HASH_TABLE_SIZE : constant := 250;
    
    -- must declare node to make access to it and then later go back and use its access...
    type Node; 
    type Node_Ptr is access all Node;

    type Node is record
        Value : Unbounded_String;
        Next  : Node_Ptr := null;
    end record;

    -- chained hash table so each array element holds node ptr
    type Hash_Table_Array is array (0 .. HASH_TABLE_SIZE - 1) of Node_Ptr;
    Hash_Table : Hash_Table_Array := (others => null); -- init to null values

    -- need to do a translate with a map as the string are unbounded
    function To_Lower_Case (S : Unbounded_String) return Unbounded_String is
    begin
        return Ada.Strings.Unbounded.Translate(S, Ada.Strings.Maps.Constants.Lower_Case_Map);
    end To_Lower_Case;

    -- total up ascii values to create hash
    function Make_Hash (Value : Unbounded_String) return Natural is
        Ascii_Total : Natural := 0;
    begin
        for C of To_String (Value) loop
            Ascii_Total := Ascii_Total + Character'Pos (C);
        end loop;
        return (Ascii_Total * 1031) mod HASH_TABLE_SIZE; -- mult by prime to spread data
    end Make_Hash;

    -- find where the hash code goes and either start a chain or add to it
    procedure Put (Str : Unbounded_String) is
        Hash_Code : constant Natural := Make_Hash (To_Lower_Case (Str));
        New_Node  : Node_Ptr := new Node'(Value => Str, Next => Hash_Table(Hash_Code));
    begin
        Hash_Table(Hash_Code) := New_Node;
    end Put;

   -- return amount of get+ it took to find a value or -1
   function Get (Str : Unbounded_String) return Integer is
      Hash_Code   : constant Natural := Make_Hash (To_Lower_Case (Str));
      Current     : Node_Ptr := Hash_Table(Hash_Code);
      Comparisons : Integer := 1;
   begin
      while Current /= null loop
         if Current.Value = Str then
            return Comparisons;
         end if;
         Current := Current.Next;
         Comparisons := Comparisons + 1;
      end loop;
      return -1;
   end Get;

   procedure Generate_Histogram is
        Pigeons : Natural;
        Pigeon_Holes : constant Natural := HASH_TABLE_SIZE;
    begin
        for I in 0 .. Pigeon_Holes - 1 loop
            Pigeons := 0;
            declare
                Current_Node : Node_Ptr := Hash_Table(I);
            begin
                while Current_Node /= null loop
                    Pigeons := Pigeons + 1;
                    Current_Node := Current_Node.Next;
                end loop;
            end;
            
            Put(Head("Pigeonhole " & Integer'Image(I) & ": ", 15));
            Put_Line((Pigeons * '*'));
        end loop;
    end Generate_Histogram;

   -- I impemented selection sort for simplicity (I'm all Ada'ed out)
   procedure Selection_Sort (Arr : in out Vector) is
      N : constant Natural := Natural(Arr.Elements.Length);
      Min_Index : Positive;
      Temp : Unbounded_String;
   begin
      for I in 1 .. N - 1 loop
         Min_Index := I;
         for J in I + 1 .. N loop
            if Arr.Elements(J) < Arr.Elements(Min_Index) then
               Min_Index := J;
            end if;
         end loop;
         if Min_Index /= I then
            Temp := Arr.Elements(I);
            Arr.Elements(I) := Arr.Elements(Min_Index);
            Arr.Elements(Min_Index) := Temp;
         end if;
      end loop;
   end Selection_Sort;

   function Sequential_Search
     (Arr : Vector; Target : Unbounded_String) return Integer
   is
   begin
      for I in 0 .. Natural(Arr.Elements.Length) - 1 loop
         if Arr.Elements(I) = Target then
            return I;
         end if;
      end loop;
      return -1;
   end Sequential_Search;

   function Binary_Search
     (Arr : Vector; Target : Unbounded_String; 
      Comparisons : out Integer) return Integer
   is
      Left : Integer := 0;
      Right : Integer := Integer (Arr.Elements.Length) - 1;
      Mid : Integer;
      Target_Lower : constant Unbounded_String := To_Lower_Case (Target);
   begin
      Comparisons := 0;
      while Left <= Right loop
         Mid := Left + (Right - Left) / 2;
         Comparisons := Comparisons + 1;

         if To_Lower_Case (Arr.Elements(Mid)) = Target_Lower then
            return Mid;
         elsif To_Lower_Case (Arr.Elements(Mid)) < Target_Lower then
            Left := Mid + 1;  -- search right half
         else
            Right := Mid - 1;  -- search left half
         end if;
      end loop;
      return -1;
   end Binary_Search;

   function Get_Magic_Items (Filename : String) return Vector is
      Magic_Items : Vector;
      File : Ada.Text_IO.File_Type;
      Line : Unbounded_String;
   begin
      -- Open the file
      begin
          Ada.Text_IO.Open (File => File,
                            Mode => Ada.Text_IO.In_File,
                            Name => Filename);
      exception
          when Ada.IO_Exceptions.Name_Error =>
            Ada.Text_IO.Put_Line ("File opening failed.");
            return Magic_Items;  -- Return empty vector
      end;

      -- Read lines from the file
      while not Ada.Text_IO.End_Of_File (File) loop
          Line := To_Unbounded_String (Ada.Text_IO.Get_Line (File));
          Magic_Items.Elements.Append (Line);
      end loop;

      -- Close the file
      Ada.Text_IO.Close (File);
      return Magic_Items;
   end Get_Magic_Items;

   Sample_Size : constant := 42;
   -- Random number generator for sampling
   subtype Random_Range is Positive range 1 .. Sample_Size;
   package Random_Integer is new Ada.Numerics.Discrete_Random (Random_Range);
   Gen : Random_Integer.Generator;

    -- Function to get a random sample
   function Get_Random_Sample (Items : Vector; Sample_Size : Positive) return Vector is
      Sample : Vector;
      Used_Indices : array (1 .. Natural(Items.Elements.Length)) of Boolean := (others => False);
      Random_Index : Positive;
   begin
      Random_Integer.Reset (Gen);  -- Initialize the random number generator
      while Natural(Sample.Elements.Length) < Sample_Size loop
         Random_Index := Random_Integer.Random (Gen) mod Natural(Items.Elements.Length) + 1;
         if not Used_Indices(Random_Index) then
            Sample.Elements.Append (Items.Elements(Random_Index));
            Used_Indices(Random_Index) := True;
         end if;
      end loop;
      return Sample;
   end Get_Random_Sample;


   MAGICITEMS_PATH : constant String := "magicitems.txt"; 

   -- Assuming these are defined in your Assignment2 package
   Magic_Items, Random_Sample : Vector;
   Magic_Table : Hash_Table_Array;

   Total_Seq_Comps, Total_Bin_Comps, Total_Hash_Get : Natural := 0;
   Binary_Comps, Hash_Get : Integer;
   Found_Idx : Integer;

begin
   Put_Line ("Hash Table Testing:");
   Put (To_Unbounded_String("Hello!"));
   Put (To_Unbounded_String("Hello!"));
   Put (To_Unbounded_String("test!"));
   Put (To_Unbounded_String("Something with a different hash"));

   declare
      Comparisons : Integer := Get (To_Unbounded_String ("Hello!"));
   begin
      if Comparisons /= -1 then
         Put_Line ("""Hello!"" was found in the table with " & Integer'Image(Comparisons) & " comparisons.");
      end if;
   end;

   if Get (To_Unbounded_String ("Not in table...")) = -1 then
      Put_Line ("""Not in table..."" was not found in the table.");
   end if;

   -- Load magic items
   Magic_Items := Get_Magic_Items (MAGICITEMS_PATH);
   Selection_Sort (Magic_Items);  -- Assuming you've implemented this

   -- Get random sample
   Random_Sample := Get_Random_Sample (Magic_Items, Sample_Size);

   -- Load hash table
   for Item of Magic_Items.Elements loop
      Put (Item);
   end loop;

   Put_Line ("Magic Items Table Visualization:");
   Generate_Histogram;

   Put_Line ("Searching for a random sample:");
   for Item of Random_Sample.Elements loop
      -- Sequential Search
      Found_Idx := Sequential_Search (Magic_Items, Item);
      if Found_Idx /= -1 then
         Put_Line ("""" & To_String(Item) & """ found with Sequential Search at idx: " & 
                   Integer'Image(Found_Idx) & ". It took " & Integer'Image(Found_Idx + 1) & " Comparisons.");
         Total_Seq_Comps := Total_Seq_Comps + Found_Idx + 1;
      else
         Put_Line ("""" & To_String(Item) & """ was not found in magicItems. Comparisons: " & 
                   Integer'Image(Natural(Magic_Items.Elements.Length)));
         Total_Seq_Comps := Total_Seq_Comps + Natural(Magic_Items.Elements.Length);
      end if;

      -- Binary Search
      Found_Idx := Binary_Search (Magic_Items, Item, Binary_Comps);
      if Found_Idx /= -1 then
         Put_Line ("""" & To_String(Item) & """ found with Binary search at idx: " & 
                   Integer'Image(Found_Idx) & ". It took " & Integer'Image(Binary_Comps) & " Comparisons.");
      else
         Put_Line ("""" & To_String(Item) & """ was not found in magicItems. Comparisons: " & 
                   Integer'Image(Binary_Comps));
      end if;
      Total_Bin_Comps := Total_Bin_Comps + Binary_Comps;

      -- Hash Table Search
      Hash_Get := Get (Item);
      if Hash_Get /= -1 then
         Put_Line ("""" & To_String(Item) & """ found with Hash Table with " & 
                   Integer'Image(Hash_Get) & " get+ comparisons.");
      else
         Put_Line ("""" & To_String(Item) & """ was not found in magicItems w/ hash table.");
      end if;
      Total_Hash_Get := Total_Hash_Get + Hash_Get;
   end loop;

   -- Print average comparisons
   Put_Line ("Sequential/Linear search took an average of " & 
             Float'Image(Float(Total_Seq_Comps) / Float(Sample_Size)) & " comparisons to find each element.");
   Put_Line ("Binary search took an average of " & 
             Float'Image(Float(Total_Bin_Comps) / Float(Sample_Size)) & " comparisons to find each element.");
   Put_Line ("Hash Table took an average of " & 
             Float'Image(Float(Total_Hash_Get) / Float(Sample_Size)) & " comparisons to find each element.");

end Assignment2;