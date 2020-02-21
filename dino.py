from m5stack import lcd
from urandom import randint

lcd.clear(0x000000)


class Game:
    def __init__(self):
        self.score = 0
        lcd.setColor(0xffffff)
        self.components = []
        self.dino = Dino(self,10,120)
        self.obs_generator = ObstacleGenerator(self,1)
        self.cloud = Cloud(80,20,0)
        self.components.append(self.dino)
        self.components.append(self.cloud)
        self.components.append(self.obs_generator)
        lcd.font(lcd.FONT_Default)
        self.create_init_ui()
    
    def render(self):
        lcd.setColor(0xffffff)
        lcd.text(0,145, str(self.score))
        for com in self.components:
            com.render()
    
    def update(self):
        for com in self.components:
            com.update()

    def create_init_ui(self):
        lcd.drawLine(0,140,80,140) 
        lcd.text(0,6, "Dino Run")
        # lcd.drawRect(0,0,80,160)
    
    def create_dead_ui(self):
        lcd.setColor(0xff0000)
        lcd.text(19,30, "Dead!")
    
    def run(self):
        while True:
            # 得分越高，游戏帧数越高，画面更新越快
            time.sleep(1/(30+self.score))
            # time.sleep(0.001)
            if buttonA.isPressed() and self.dino.act == 0:
                self.dino.setAct(1)
                self.obs_generator.on()
            elif buttonA.isPressed() and self.dino.act == 1:
                self.dino.setAct(2)
            if buttonB.isPressed():
                lcd.clear()
                self = Game()
            if self.collision_check():
                self.create_dead_ui()
            else:
                self.update()
                self.render()

            
    
    def collision_check(self):
        # [鼻子，右脚，左脚，尾巴尖端]
        if self.dino.get_collision_pixels()[0][0] < self.obs_generator.get_collision_box()[0]:
            # 如果障碍物在鼻子前面，则不需要进行判断碰撞，直接返回False
            # print("障碍物在鼻子前面")
            return False
        elif self.dino.get_collision_pixels()[3][0] > self.obs_generator.get_collision_box()[0] + self.obs_generator.get_collision_box()[2]:
            # 如果障碍物在尾巴后面，则不需要进行判断碰撞，直接返回False
            # print("障碍物在尾巴后面")
            return False
        elif self.dino.get_collision_pixels()[1][1] < self.obs_generator.get_collision_box()[1]:
            # 如果障碍物在双脚下面，则不需要进行判断碰撞，直接返回False
            # print("障碍物在双脚下面")
            return False
        # 接下来就是判断小恐龙的检测点是否位于障碍物内了
        # print("enter Loop Check")
        for hit_pixel in self.dino.get_collision_pixels():
            if hit_pixel[0] > self.obs_generator.get_collision_box()[0] and  \
                hit_pixel[0] < self.obs_generator.get_collision_box()[0] + self.obs_generator.get_collision_box()[2] and \
                hit_pixel[1] < self.obs_generator.get_collision_box()[1] +  self.obs_generator.get_collision_box()[3] and \
                hit_pixel[1] > self.obs_generator.get_collision_box()[1] :
                return True
        
        # 由于我产生了一种错觉..那就是如果上述三种情况均未触发，那就说明发生了碰撞.
        # 结果几次验证，发现这个猜想符合.. 
        return False


class Obstacle:

    def __init__(self, game, x, y, type):
        self.game = game
        self.x = x
        self.y = y
        self.vx = 4
        self.old_x = x
        # 多种类型的仙人掌，高度不同
        self.type = type
        self.obs = [
            [
                [3,1,3,25],
                [1,8,2,8],
                [7,4,1,7],
                [6,10,2,2]
            ],
            [
                [3,11,3,15], 
                [0,15,2,4],
                [0,18,4,3],
                [7,12,2,4],
                [6,15,3,3]
            ],
            [
                [3,6,3,20], 
                [0,12,2,4],
                [0,16,4,3],
                [7,7,2,7],
                [6,13,3,3]
            ],
            [
                [3,15,2,11], 
                [0,15,1,4],
                [0,19,4,2],
                [7,19,1,5],
                [5,21,2,2]
            ]
        ]
        # 轮廓，用于碰撞检测
        self.collision = [
            [1,1,9,25],
            [0,11,9,15],
            [0,6,9,20],
            [0,15,8,11]
        ]
    
    def update(self):
        self.old_x = self.x
        self.x -= self.vx
    
    def get_collision_box(self):
        # 返回一个最新的Collision
        return [self.collision[self.type][0] + self.x, self.y + self.collision[self.type][1], self.collision[self.type][2], self.collision[self.type][3]]

    def off_screen(self):
        return self.x < -(self.collision[self.type][2] - self.collision[self.type][0])

    def render(self):
        # 清空上一帧内容
        lcd.setColor(0)
        for ob in self.obs[self.type]:
            lcd.fillRect(self.old_x + ob[0], self.y + ob[1], ob[2], ob[3])
        
        # lcd.drawRect(self.collision[self.type][0]+ self.old_x, self.collision[self.type][1] + self.y, self.collision[self.type][2], self.collision[self.type][3])
        if not self.off_screen():
            # 绘制当前帧内容
            lcd.setColor(0x00ff00)
            for ob in self.obs[self.type]:
                lcd.fillRect(self.x + ob[0], self.y + ob[1], ob[2], ob[3])
            # lcd.setColor(0xff0000)
            # lcd.drawRect(self.collision[self.type][0] + self.x, self.y + self.collision[self.type][1], self.collision[self.type][2], self.collision[self.type][3])
        

class Cloud:
    def __init__(self, x, y, type):
        self.x = x
        self.y = y
        self.vx = 1
        self.type = type
        self.old_x = x
        # lcd.clear()
        # lcd.setColor(0xffffff)

        # lcd.fillRoundRect(10,120, 20,4,2)
        # lcd.fillRoundRect(20,123, 4,4,2)
        # lcd.fillRoundRect(15,127, 14,4,2)
        self.roundRect = [
            [
                [1,0,20,4,2],
                [10,3,4,4,2],
                [5,7,14,4,2]
            ],
            [
                [10,0,10,4,2],
                [14,3,4,4,2],
                [5,7,14,4,2]
            ],
            [
                [5,0,15,4,2],
                [9,3,2,4,2],
                [1,7,19,4,2]
            ]
        ]
    
    def off_screen(self):
        # 云的宽度都是20
        return self.x < -20

    def update(self):
        self.old_x = self.x
        self.x -= self.vx
            
    
    def render(self):
        lcd.setColor(0)
        for rect in self.roundRect[self.type]:
            lcd.fillRoundRect(self.old_x + rect[0], self.y + rect[1], rect[2], rect[3],rect[4])
        lcd.setColor(0x999999)
        if not self.off_screen():
            for rect in self.roundRect[self.type]:
                lcd.fillRoundRect(self.x + rect[0], self.y + rect[1], rect[2], rect[3],rect[4])
        else:
            # 重新生成一片云
            self.type = randint(0,2)
            self.x = randint(80,100)
            self.y = randint(30,60)
            self.vx = randint(1,3)

class ObstacleGenerator:

    def __init__(self, game, level):
        self.game = game
        self.level = level
        self.type = randint(0,3)
        self.obs = Obstacle(self.game, randint(120,140), 114,  self.type)
        self.active = False

    def on(self):
        self.active = True
        lcd.clear()
        self.game.create_init_ui()
        
    
    def render(self):
        if self.active:
            self.obs.render()
        else:
            lcd.setColor(0xffffff)
            lcd.text(20, 40, "Press")
            lcd.text(31, 55, "M5")
            lcd.text(22, 70, "Start")
            lcd.text(20, 85, "Game")
    
    def update(self):
        if self.active:
            # 障碍物离开屏幕，就得分，然后随机生成下一个障碍物
            if self.obs.off_screen():
                self.game.score += 1
                self.type = randint(0,3)
                self.obs = Obstacle(self.game, randint(80,120), 114, self.type)
            else:
                self.obs.update()

    def get_collision_box(self):
        return self.obs.get_collision_box()

class Dino:
    def __init__(self, game, x, y):
        self.game = game
        self.x = x
        self.y = y
        self.vy = 9
        self.old_x = x
        self.old_y = y
        self.base_line = y
        # leg_status代表脚的状态
        # 0 代表不动
        # 1 迈出左脚
        # 2 迈出右脚
        self.leg_status = 0
        # 重力加速度
        self.g = 1
        # act 代表小恐龙状态
        # 0 闲置
        # 1 跑
        # 2 跳跃上升
        # 3 跳跃下降
        # 4 落地前一瞬间
        self.act = 0
        # 眨眼控制
        self.blink = 1
        # [start_x, start_y, end_x, end_y]
        self.head_lines = [
            [0,7,0,11],
            [1,9,1,12],
            [2,10,2,13],
            [6,9,6,11],
            [5,10,9,7],
            [11,0,19,0],
            ## Eye #
            [10,1,12,1],
            [13,1,19,1],
            ## Eye #
            [13,6,16,6],
            [13,9,14,9],
            [14,9,16,9],
            [4,15,11,15]
        ]
        # [Left, Top, Width, Height]
        self.body_rects = [
            [3,11,10,4],
            # 下面那个矩形有毒，绘制以后缺少一个像素点
            [7,8,3,3],
            [10,2,4,9],
            [14,2,6,3]
        ]
        # [x, y]
        self.extra_pixels = [
            # [10,10]
            [9,10],
            [5,10],
            [15,10]
        ]
    
    def get_collision_pixels(self):
        # 返回四个特殊顶点
        # 鼻子，右脚，左脚，尾巴尖端
        # 绝对坐标[x, y]
        return [
            [self.x + 19, self.y + 1],
            [self.x + 11, self.y + 19],
            [self.x + 5, self.y + 19],
            [self.x, self.y + 11]
        ]

    def update(self):
        self.old_x = self.x
        self.old_y = self.y
        # 更新脚部动画计数器
        if self.act == 1:
            self.leg_status += 1
        if self.act == 2:
            self.leg_status = 30
            self.y -= self.vy
            self.vy -= self.g
            # 到达最高点
            # 重置状态为落地状态
            if self.vy == 0:
                self.act = 3
                self.vy = 3
        elif self.act == 3:
            self.leg_status = 30
            self.y += self.vy
            #  受重力影响，落地速度更快
            self.vy += self.g
            if self.y >= self.base_line - 3:
                # print("land on")
                # 当速度为0，则落地，重置状态为跑
                # 重置Y轴速度为-5
                self.y = self.base_line
                self.act = 4
                self.vy = 9
    
    def draw_leg(self):
        # 脚相关的坐标
        # 大腿
        rects = [
            [5,16,2,2],
            [9,16,2,2]
        ]
        # 正常左脚
        left_nor_leg = [
            [5,18],
            [5,19],
            [6,19],
            [7,16]
        ]
        # 正常右脚
        right_nor_leg = [
            [10,18],
            [10,19],
            [11,19]
        ]
        # 左脚抬起相关的坐标
        up_left_leg = [
            [5,16,5,18],
            [5,17,7,17]
        ]
        # 右脚抬起相关的坐标
        up_right_leg = [
            [10,16,10,18],
            [11,17,12,17]
        ]
        # 换脚的时候清空上一帧抬起的脚
        # 绘制静止状态的脚
        if self.act != 1:
            for rect in rects:
                lcd.fillRect(self.x + rect[0], self.y + rect[1], rect[2], rect[3])
            for pixel in left_nor_leg:
                lcd.drawPixel(self.x + pixel[0], self.y + pixel[1])
            for pixel in right_nor_leg:
                lcd.drawPixel(self.x + pixel[0], self.y + pixel[1])
        else:
            # 跑步
            # 抬起左脚
            if self.leg_status < 10:
                lcd.fillRect(self.x + rects[1][0], self.y + rects[1][1], rects[1][2], rects[1][3])
                for pixel in right_nor_leg:
                    lcd.drawPixel(self.x + pixel[0], self.y + pixel[1])
                for left_leg in up_left_leg:
                    lcd.drawLine(self.x + left_leg[0], self.y + left_leg[1], self.x + left_leg[2], self.y+left_leg[3])
            # 抬起右脚
            elif self.leg_status < 20:
                lcd.fillRect(self.x + rects[0][0], self.y + rects[0][1], rects[0][2], rects[0][3])
                for pixel in left_nor_leg:
                    lcd.drawPixel(self.x + pixel[0], self.y + pixel[1])
                for right_leg in up_right_leg:
                    lcd.drawLine(self.x + right_leg[0], self.y + right_leg[1], self.x + right_leg[2], self.y+right_leg[3])
            else:
                self.leg_status = 0
        

    def clear_leg(self):
        lcd.setColor(0)
        lcd.fillRect(self.old_x + 5, self.old_y + 16, 10,4)

    def setAct(self, act):
        self.act = act


    def render(self):
        lcd.setColor(0)
        if self.act == 2 or self.act == 3 or self.act == 4:
            # 跳跃状态才需要清空之前一帧绘制的内容
            # 清空上一帧绘制的内容
            for line in self.head_lines:
                lcd.drawLine(self.old_x + line[0], self.old_y + line[1], self.old_x + line[2], self.old_y + line[3])
            for rect in self.body_rects:
                lcd.fillRect(self.old_x + rect[0], self.old_y + rect[1], rect[2], rect[3])
            for pixel in self.extra_pixels:
                lcd.drawPixel(self.old_x + pixel[0], self.old_y + pixel[1])
            # 清空运动中上一帧眨眼的坐标
            lcd.drawPixel(self.old_x + 12, self.old_y + 1)
            # self.act == 4这是从跳跃状态转移至普通状态的中间状态，
            # 此时仍然需要清空上一帧内容，不然会导致状态错误而留下残影
            if self.act == 4:
                # 清空完上一帧内容，再去转变状态
                self.act = 1
                self.leg_status = 0
        self.clear_leg()
        # 绘制本一帧的内容
        lcd.setColor(0xffffff)
        for line in self.head_lines:
            lcd.drawLine(self.x + line[0], self.y + line[1], self.x + line[2], self.y + line[3])
        for rect in self.body_rects:
            lcd.fillRect(self.x + rect[0], self.y + rect[1], rect[2], rect[3])
        for pixel in self.extra_pixels:
            lcd.drawPixel(self.x + pixel[0], self.y + pixel[1])
        # 由于不断更新的，所以脚单独绘制
        self.draw_leg()
        if self.blink < 40:
            lcd.setColor(0)
            lcd.drawPixel(self.x + 12, self.y + 1)
        elif self.blink < 60:
            # 闭眼效果在此
            lcd.setColor(0xffffff)
            lcd.drawPixel(self.x + 12, self.y + 1)
        else:
            self.blink = 0
        self.blink += 1
        # lcd.setColor(0xff0000)
        # for p in self.get_collision_pixels():
        #     lcd.drawPixel(p[0], p[1])


game = Game()

game.run()




# x, y radius width s_start_angle, end_angle
