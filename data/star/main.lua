initstars()
creobj()

colangle = 0
-- sw = 1

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
  zoom = 0.5
}

light = {
  x = 1,
  y = 3,
  z = 10
}

function _init()

end

function _update()


  -- if btnp(7) then 
  --   if camera.zoom > 0.05 then
  --   camera.zoom = camera.zoom + 0.05  
  --   end
  -- end
  -- if btnp(8) then 
  --   if camera.zoom < 1.0 then
  --   camera.zoom = camera.zoom - 0.05  
  --   end
  -- end


  -- if btnp(1) then 
  --   camera.angley  = camera.angley - 2
  -- end
  -- if btnp(2) then 
  --   camera.angley  = camera.angley + 2
  -- end


  -- if btnp(3) then 
  --   -- sw = 1
  --   camera.anglex  = camera.anglex - 2
  -- end

  -- if btnp(4) then   
  --   -- sw = 1
  --   camera.anglex  = camera.anglex + 2
  -- end


  -- if btnp(6) then 
  --   sw = 2
  --   camera.angley  = camera.angley - 2
  -- end
  -- if btnp(5) then   
  --   sw = 2
  --   camera.angley  = camera.angley + 2
  -- end

  if btn(1) >= 2 then   
    -- camera.angley  = camera.angley + 2
    camera.zoom = camera.zoom - 0.5
  end

  if btn(2) >= 2 then 
    -- camera.angley  = camera.angley - 2
    camera.zoom = camera.zoom + 0.5
  end

  if btn(3) >= 2 then   
    camera.anglex  = camera.anglex - 2
  end

  if btn(4) >= 2 then 
    camera.anglex  = camera.anglex + 2
  end

  camera.angley = camera.angley + 1;
  camera.zoom = 0.7;
  

end 

function _draw()
  cls()

  drawstars( camera.anglex, camera.angley, camera.zoom, 0)
  -- drawstars( camera.anglex, camera.angley, sw, camera.zoom)
end
