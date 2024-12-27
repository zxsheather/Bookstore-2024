import customtkinter as ctk
import tkinter as tk

class App(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("zxsheather's bookstore")
        self.geometry("600x300")
        self.configure(fg_color="white")  

        # 配置主窗口的行和列，使所有 Frame 填满整个窗口
        self.grid_rowconfigure(0, weight=1)
        self.grid_columnconfigure(0, weight=1)

        # 创建一个容器来存放所有的 frame
        container = ctk.CTkFrame(self, fg_color="white")
        container.grid(row=0, column=0, sticky="nsew")

        # 配置容器的行和列，使其中的 Frame 可以填满容器
        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        # 存储所有的 frame
        self.frames = {}

        for F in (InitialFrame, LoginFrame, RegisterFrame):
            frame = F(parent=container, controller=self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(InitialFrame)

    def show_frame(self, frame_class):
        frame = self.frames[frame_class]
        frame.tkraise()

class InitialFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")

        label = ctk.CTkLabel(
            self, 
            text="Welcome to Zxsheather's Bookstore!", 
            font=("Monaco", 25), 
            text_color="black"
        )
        label.pack(pady=20)

        btn_login = ctk.CTkButton(
            self, 
            text="Login", 
            command=lambda: controller.show_frame(LoginFrame),
            corner_radius=20,            
            fg_color="lightgreen",       
            text_color="black",          
            width=200,
            height=50,
            font=("Arial", 14)
        )
        btn_login.pack(pady=20)

        btn_register = ctk.CTkButton(
            self, 
            text="Register", 
            command=lambda: controller.show_frame(RegisterFrame),
            corner_radius=20,            
            fg_color="lightblue",       
            text_color="black",          
            width=200,
            height=50,
            font=("Arial", 14)
        )
        btn_register.pack(pady=20)

class PlaceholderEntry(ctk.CTkEntry):
    def __init__(self, master=None, placeholder="PLACEHOLDER", color_placeholder="grey", *args, **kwargs):
        super().__init__(master, *args, **kwargs)
        self.placeholder = placeholder
        self.color_placeholder = color_placeholder
        self.default_fg_color = self.cget("text_color")
        
        self.put_placeholder()
        
        self.bind("<FocusIn>", self.foc_in)
        self.bind("<FocusOut>", self.foc_out)
    
    def put_placeholder(self):
        self.insert(0, self.placeholder)
        self.configure(text_color=self.color_placeholder)
    
    def foc_in(self, *args):
        if self.get() == self.placeholder:
            self.delete(0, 'end')
            self.configure(text_color=self.default_fg_color)
    
    def foc_out(self, *args):
        if not self.get():
            self.put_placeholder()

class LoginFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        
        # 配置网格为3行3列
        self.grid_rowconfigure(0, weight=1)  
        self.grid_rowconfigure(1, weight=0)  
        self.grid_rowconfigure(2, weight=1)  
          

        self.grid_columnconfigure(0, weight=1)  # 左侧空白列
        self.grid_columnconfigure(1, weight=0)  # 内容列

        # 返回按钮
        btn_back = ctk.CTkButton(
            self, 
            text="Return", 
            command=lambda: controller.show_frame(InitialFrame),
            corner_radius=20,            
            fg_color="lightgray",       
            text_color="black",          
            width=100,
            height=40,
            font=("Arial", 12)
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        # 登录标签
        label = ctk.CTkLabel(
            master=self,
            text="Login Surface", 
            font=("Monaco", 25), 
            text_color="black"
        )
        label.grid(row=0, column=0, padx=20, pady=20, sticky="ew", columnspan=2, rowspan=2)
        
        # 用户ID输入框
        entry_userid = PlaceholderEntry(
            self, 
            placeholder="Userid:",
            width=300,
            font=("Arial", 14)
        )
        entry_userid.grid(row=1, column=0, padx=20, pady=20, columnspan=2)
        
        # 密码输入框
        entry_password = PlaceholderEntry(
            self,
            placeholder="Password:",
            width=300,
            font=("Arial", 14)
        )
        entry_password.grid(row=2, column=0, padx=20, pady=20, columnspan=2,sticky="n")
class RegisterFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        
        self.grid_rowconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, 
            text="Register Surface", 
            font=("Monaco", 25), 
            text_color="black"
        )
        label.grid(row=0, column=0, padx=20, pady=20,columnspan=2,rowspan=2)

        btn_back = ctk.CTkButton(
            self, 
            text="Return", 
            command=lambda: controller.show_frame(InitialFrame),
            corner_radius=20,            
            fg_color="lightgray",       
            text_color="black",          
            width=100,
            height=40,
            font=("Arial", 14)
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20,sticky="nw")

        entry_userid = PlaceholderEntry(
            self, 
            placeholder="Userid:",
            width=300,
            font=("Arial", 14)
        )
        entry_userid.grid(row=1, column=0, padx=20, pady=20, columnspan=2)
        
        entry_password = PlaceholderEntry(
            self,
            placeholder="Password:",
            width=300,
            font=("Arial", 14)
        )
        entry_password.grid(row=2, column=0, padx=20, pady=20, columnspan=2,sticky="n")

        entry_username = PlaceholderEntry(
            self,
            placeholder="Username:",
            width=300,
            font=("Arial", 14)
        )
        entry_username.grid(row=3, column=0, padx=20, pady=20, columnspan=2,sticky="n")


if __name__ == "__main__":
    app = App()
    app.mainloop()