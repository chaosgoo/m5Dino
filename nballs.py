from m5stack import lcd
lcd.clear(lcd.BLACK)
import time
from urandom import randint
# Rect.LTWH()

class Ball:
    def __init__(self,x,y,r):
        # 初始化位置及球体大小
        self.x = x
        self.y = y
        self.r = r
        self.vx = randint(1,4)
        self.vy = randint(1,4)
        self.epslion = 0.99
        self.old_x = x
        self.old_y = y
        self.color = randint(0,16579836)
    def update(self):
        # 保存旧参数
        self.old_x = self.x
        self.old_y = self.y
        # if self.x+self.vx - self.r<0:
        #     self.x = 0
        #     self.vx = -self.vx
        # elif self.x+self.vx+self.r <= 80:
        #     self.x += self.vx
        # else:
        #     self.x = 80
        #     self.vx = -self.vx
        # if self.y+self.vy -self.r< 0:
        #     self.y = 0
        #     self.vy = -self.vy
        # elif self.y+self.vy+self.r <= 160:
        #     self.y += self.vy
        # else:
        #     self.y = 160
        #     self.vy = -self.vy
        # 更新位置
        self.x = self.x + self.vx
        self.y = self.y + self.vy
        # 边界碰撞检测
        if self.x - self.r <= 0 or self.x + self.r > 80:
            self.vx = -self.vx
        if self.y - self.r <= 0 or self.y + self.r > 160:
            self.vy = -self.vy
    def render(self):
        # 绘制图像
        self.clear()
        lcd.setColor(self.color)
        lcd.fillCircle(self.x, self.y, self.r)

        self.old_x = self.x
        self.old_y = self.y
    def clear(self):
        # 清空上一帧内容
        lcd.setColor(lcd.BLACK)
        lcd.fillCircle(self.old_x, self.old_y, self.r)

balls = []
# for _ in range(randint(10,15)):
    # balls.append(Ball(randint(6,80),randint(6,160),randint(1,5)))
print(len(balls))
press_delay = 0
while True:
    press_delay +=1
    # print(press_delay)
    if buttonA.read() and press_delay > 10:
        balls.append(Ball(randint(6,75),randint(6,155),randint(1,5)))
        press_delay = 0
    # lcd.clear(lcd.BLACK)
    if buttonB.read() and press_delay > 10:
        press_delay = 0
        if len(balls) != 0:
            balls[0].clear()
            balls.pop(0)
    # time.sleep(0.033)
    for ball in balls:
        ball.update()
        ball.render()
    
