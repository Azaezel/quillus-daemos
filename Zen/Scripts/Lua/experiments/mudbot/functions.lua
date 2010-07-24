function explode(arg,str)
  if (arg=='') then return false end
  local pos,arr = 0,{}
  -- for each divider found
  for st,sp in function() return string.find(str,arg,pos,true) end do
    table.insert(arr,string.sub(str,pos,st-1))
    pos = sp + 1 
  end
  table.insert(arr,string.sub(str,pos)) 
  return arr
end
