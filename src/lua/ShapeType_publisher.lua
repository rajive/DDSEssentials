--*****************************************************************************
--*    (c) 2005-2017 Copyright, Real-Time Innovations, All rights reserved.   *
--*                                                                           *
--*         Permission to modify and use for internal purposes granted.       *
--* This software is provided "as is", without warranty, express or implied.  *
--*  
--* Author: Rajive Joshi, Real-Time Innovations (RTI)                                                                        *
--*****************************************************************************

---ShapeType_publisher
--    Publish a shape (make a circular pattern)
-- 
-- HOW TO?
--  rtiddsprototyper -cfgName MyServiceIfLib::MyService.Pub -luaFile src/lua/ShapeType_publisher.lua
--
--  ShapesDemo: 
--    Subscribe to the shape

-- Globals (preserved across invocations)
if count then count = count + 1 
else -- initialize (first time)
  count = 0
  center = 120; radius = 70; yAmplitude = 100 
  MyWriter = CONTAINER.WRITER['MyPublisher::MyWriter']
end

print("*** iteration ", count,  "***")

MyWriter.instance['color'] = "YELLOW"
MyWriter.instance['x'] = center + radius * math.sin(count)
MyWriter.instance['y'] = center + radius * math.cos(count) 
MyWriter.instance['shapesize'] = 20 -- 10 * (count % 4)

MyWriter:write()
