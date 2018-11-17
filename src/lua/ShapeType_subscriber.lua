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
--  rtiddsprototyper -cfgName MyServiceIfLib::MyService.Sub -luaFile lua/ShapeType_subscriber.lua
--
--  ShapesDemo: 
--    Publish the shape

-- Globals (preserved across invocations)
if count then count = count + 1 
else 
  count = 0
  MyReader = CONTAINER.READER['MySubscriber::MyReader']
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
