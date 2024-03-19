flag = true

function draw_mandelbrot(x_min, x_max, y_min, y_max, max_iterations)
  w=128 -- 画面サイズ
  h=128 -- 画面サイズ
  x_step = (x_max - x_min) / w -- x方向の増分
  y_step = (y_max - y_min) / h -- y方向の増分
  
  for j=0, h-1 do
    for i=0, w-1 do
      -- ピクセル座標を複素数に変換する
      x = x_min + i * x_step
      y = y_min + j * y_step
      c = {x, y}
      
      -- マンデルブロ集合に属するかどうかを判定する
      z = {0, 0}
      k = 0
      
      while k < max_iterations and z[1]^2 + z[2]^2 < 4 do
        new_z = {z[1]^2 - z[2]^2 + c[1], 2 * z[1] * z[2] + c[2]}
        z = new_z
        k = k + 1
      end
      
      -- マンデルブロ集合に属する場合は白色で塗りつぶす
      if k == max_iterations then
        pset(i, j, 255,255,255)
      end
    end
  end
end

-------------------------------------------------------

function setup()

end

function _init()--1回だけ
    setup()
end

function input()
  -- x = tp(0)
  -- y = tp(1)
  pinw(0) --led消灯

  if btn(1) >= 1 then
    tone(0, 440, 128)
  end
  if btn(2) >= 2 then
    -- tone(0, 440, 0)
    pinw(1)--led点灯
  end

end

function _update()--ループします
  input()
  
end

function _draw()--ループします

  if flag then
    -- cls()
    draw_mandelbrot(-2, 1, -1.5, 1.5, 50)
    flag = false
  end
end








  