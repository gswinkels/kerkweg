program XRSTP5(input,output);
{$M 16384,0,65536}
uses SORTBTPU;
type
  stype     = string[255];

var
  io_str            : stype;

  f1                : text;
  func,
  ret_code,
  io_len            : integer;

begin {XRSTP5}
  {***************** init the RAAT sort routine *******************}
  func   := 1;
  io_str := 'S(1,10,c,a)';
  io_len := length(io_str);
  RSBTP(func, io_str[1], io_len, ret_code);

  if ret_code <> 0
  then 
    begin
      writeln('%ERR-Error number ', ret_code, ' in sort initialization.');
      HALT(ret_code);
    end;

  {****************** send records to sort ************************}
  {$I-}
  assign(f1, 'SAMP.DAT');
  reset(f1);
  if IOResult <> 0
  then 
    begin
      writeln('%ERR - Error opening SAMP.DAT.');
      HALT(1);
    end;
  {$I+}

  func := 2;
  while NOT EOF(f1) do
    begin
      readln(f1, io_str);

      io_len := length(io_str);
      RSBTP(func, io_str[1], io_len, ret_code);

      if ret_code <> 0
      then 
        begin
          writeln('%ERR - Error ', ret_code, ' while inserting records to sort.');
          HALT(ret_code);
        end;
    end;
  close(f1);

  {****************** get records from sort ***********************}
  func := 3;

  assign(f1, '');
  rewrite(f1);

  RSBTP(func, io_str[1], io_len, ret_code);
  while (ret_code = 0) do
    begin
      io_str[0] := chr(io_len);
      writeln(f1, io_str);
      RSBTP(func, io_str[1], io_len, ret_code);
    end;
  close(f1);

  if ret_code <> 1
  then 
    begin
      writeln('%ERR-Error ', ret_code, ' retrieving records from sort.');
      HALT(ret_code);
    end;

  writeln('%MSG - XRSTP5 ended successfully.');
end   {XRSTP5}.
