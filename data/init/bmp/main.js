setup = function(){
}
c = 0
loop = function(){
    color(0,0,0)
    fillrect(0,0,128,128)
    color(255,255,255)
    text("Hello JavaScript", 10, c%128)
    c ++
}
