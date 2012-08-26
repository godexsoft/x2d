x2d.log("creating object...")

-- now play with its properties a bit
self:bgcolor(color_info(math.random(),
                        math.random(),
                        math.random(), 0.2))
self:rotation(math.random(0,360))
self:scale(math.random(0.1,2.0))
self:position(vec2(math.random(-200, 200),
                   math.random(-200, 200)))