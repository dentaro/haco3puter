p = 0.5
s = 0.2

creobj2(1,   -p,-p*2,0.0,  90,127,192, s)--cmy
creobj2(1,  0.0,-p*2,0.0, 210,127,128, s)
creobj2(1,    p,-p*2,0.0, 330,127, 96, s)

creobj2(1,   -p,-p,0.0,  60,127,128, s)--cmy
creobj2(1,  0.0,-p,0.0, 180,127,128, s)
creobj2(1,    p,-p,0.0, 300,127,128, s)

creobj2(1,   -p,0.0,0.0,   0,127,128, s)--rgb
creobj2(1,  0.0,0.0,0.0, 120,127,128, s)
creobj2(1,    p,0.0,0.0, 240,127,128, s)

colangle = 0
sw = 1

camera = {
  x = 0,
  y = 0,
  z = 0,
  x2 = 0,
  y2 = 0,
  z2 = -1,
  anglex = 1,
  angley = 0,
  anglez = 0,
  zoom = 0.1
}

light = {
  x = 1,
  y = 3,
  z = 10
}

function _init()
  camera.zoom = 1.0
  camera.z = 1.0;
end

function _update()
  -- if btnp(3) then 
  --   -- creobj2(0)
  --   camera.zoom = camera.zoom - 0.05
  -- end
  -- if btnp(4) then 
  --   -- creobj2(1)
  --   camera.zoom = camera.zoom + 0.05
  -- end

  -- if btnp(3) then 
  --   camera.z = camera.z - 0.01  
  -- end
  -- if btnp(4) then 
  --   camera.z = camera.z + 0.01  
  -- end


  if camera.zoom<0 then
    camera.zoom =0.01
  end

  

  if btnp(1) then 
    sw = 1
    camera.x  = camera.x - 0.05
  end

  if btnp(2) then   
    sw = 1
    camera.x  = camera.x + 0.05
  end


  if btnp(3) then 
    sw = 2
    camera.y  = camera.y + 0.05
  end
  if btnp(4) then   
    sw = 2
    camera.y  = camera.y - 0.05
  end
  

end 

function _draw()
  cls(8)
  rendr2( camera.x, camera.y, camera.z, camera.anglex, camera.angley, sw, camera.zoom)
end