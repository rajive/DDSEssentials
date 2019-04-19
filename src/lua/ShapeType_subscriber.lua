--*****************************************************************************
--*    (c) 2005-2017 Copyright, Real-Time Innovations, All rights reserved.   *
--*                                                                           *
--*         Permission to modify and use for internal purposes granted.       *
--* This software is provided "as is", without warranty, express or implied.  *
--*  
--* Author: Rajive Joshi, Real-Time Innovations (RTI)                                                                        *
--*****************************************************************************

---ShapeType_subscriber
--    Subscribe to a shape 
-- 
-- HOW TO?
-- 
-- Polled
--  rtiddsprototyper -cfgName ShapesIfLib::Shapes_Sub -period 4 -luaOnData false -luaFile src/lua/ShapeType_subscriber.lua
--          
-- Event         
--  rtiddsprototyper -cfgName ShapesIfLib::Shapes_Sub -luaOnPeriod false -luaFile src/lua/ShapeType_subscriber.lua
--  
-- Polled or Event:
--  rtiddsprototyper -cfgName ShapesIfLib::Shapes_Sub -luaFile src/lua/ShapeType_subscriber.lua
--                                      
--
--  ShapesDemo: 
--    Publish the shape

-- Globals (preserved across invocations)
if count then count = count + 1 
else 
  count = 0
  MyReader = CONTAINER.READER['_sub::Shape_reader']
end

print("*** iteration ", count,  "***")

MyReader:take()
for  i, shape in ipairs(MyReader.samples) do

  print("\t color:", shape['color']) -- key

  if (not MyReader.infos[i].valid_data) then
    print("\t invalid data!")
  end

  print("\t x:", shape['x']) 
  print("\t y:", shape['y'])
  print("\t shapesize:", shape['shapesize']) 
  
end
