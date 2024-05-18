local vm = VM.new()

local program = "++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++."

vm:load(program)

function clear()
   io.write("\027[2J")
end

function resetCursor()
   io.write("\027[0;0H")
end

function printProgram(vm)
   local program = vm:program()
   local indicatorLine = "";

   for i = 1, string.len(program[1]) do
      if (i == program[2]) then
         indicatorLine = indicatorLine .. "^"
      else
         indicatorLine = indicatorLine .. " "
      end
   end

   clear()
   resetCursor()
   print("Code:")
   print(program[1])
   print(indicatorLine)
   print("Data:")
   printData(vm)
   print("Done: " .. tostring(vm:done()));
   print("Input: " .. vm:getInput() .. " Output: " .. vm:getOutput())
end


function printData(vm)
   local data = vm:data()
   local dataString = ""
   local indicatorLine = ""
   
   for i = 1, #data[1] do
      dataString = dataString .. string.format("%03d ", data[1][i]);
      if (i == data[2]) then
         indicatorLine = indicatorLine .. " ^  "
      else
         indicatorLine = indicatorLine .. "    "
      end
   end

   print(dataString)
   print(indicatorLine)
end

function debugger()
   local breakpoints = {}
   local response = ""
   local previousInput = ""
   local exec = false
   
   while not vm:done() do
      printProgram(vm)

      if (exec and not breakpoints[vm:currentInstruction()]) then
         vm:crank(1)
      else 
         print("n(ext) | b(reak) X | d(elete) X| i(nput) | q(uit) | f(orward) | (b)a(ck) | e(xec)")
         io.write("> ")

         exec = false
         response = io.read()

         if (response == "" and previousInput ~= "") then
            response = previousInput
         end

         previousInput = response
         
         if (response == 'n' or response == 'next') then
            vm:crank(1)
         elseif (response:find("^b") ~= nil) then
            if response:find("^break ") ~= nil then
               breakpoints[string.sub(response, #"break ", #"break ")] = true
            else
               breakpoints[string.sub(response, 3,3)] = true
            end
         elseif (response:find("^d") ~= nil) then
            if response:find("^delete ") ~= nil then
               breakpoints[string.sub(response, #"delete ", #"delete ")] = false
            else
               breakpoints[string.sub(response, 3,3)] = false
            end
         elseif (response == 'i' or response == 'input') then
            io.write("Input > ")
            local input = io.read()
            vm:setInput(input)
         elseif (response == 'q' or response == 'quit') then
            break;
         elseif (response == 'f' or response == 'forward') then
            vm:next()
         elseif (response == 'a' or response == 'back') then
            vm:prev()
         elseif (response == 'e' or response == 'exec') then
            exec = true
            vm:crank(1)
         end
      end
   end
   printProgram(vm)
end

debugger()
