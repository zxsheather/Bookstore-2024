import customtkinter as ctk
import tkinter
from tkinter import messagebox
import os
import subprocess
import atexit
import threading

is_open = False
process = None

def exit():
    if process:
        try:
            process.stdin.write("exit\n")
            process.stdin.flush()
            process.terminate()
        except Exception as e:
            print(f"退出时出错: {e}")


class App(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("zxsheather's bookstore")
        self.geometry("600x500")
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

        for F in (
            InitialFrame,
            LoginFrame,
            RegisterFrame,
            FunctionFrame,
            SearchFrame,
            ResultFrame,
            UseraddFrame,
            PasswdFrame,
            DeleteFrame,
            SelectFrame,
            ModifyFrame,
            ImportFrame,
            BuyFrame,
        ):
            frame = F(parent=container, controller=self)
            self.frames[F] = frame
            frame.grid(row=0, column=0, sticky="nsew")

        self.show_frame(InitialFrame)

    def show_frame(self, cont):
        print(f"切换到 {cont.__name__}")  # 调试输出
        frame = self.frames.get(cont)
        if frame:
            frame.tkraise()
        else:
            print(f"没有找到 Frame: {cont.__name__}")


class InitialFrame(ctk.CTkFrame):
    input = ""

    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")

        self.grid_rowconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=1)
        self.grid_rowconfigure(2, weight=1)
        self.grid_columnconfigure(0, weight=1)
        label = ctk.CTkLabel(
            self,
            text="Welcome to Zxsheather's Bookstore!",
            font=("Monaco", 25),
            text_color="black",
        )
        label.grid(row=0, column=0, padx=20, pady=20)

        btn_login = ctk.CTkButton(
            self,
            text="Login",
            command=lambda: controller.show_frame(LoginFrame),
            corner_radius=20,
            fg_color="lightgreen",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_login.grid(row=1, column=0, padx=20, pady=20,sticky="s")

        btn_register = ctk.CTkButton(
            self,
            text="Register",
            command=lambda: controller.show_frame(RegisterFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_register.grid(row=2, column=0, padx=20, pady=20,sticky="n")


class PlaceholderEntry(ctk.CTkEntry):
    def __init__(
        self,
        master=None,
        placeholder="PLACEHOLDER",
        color_placeholder="grey",
        *args,
        **kwargs,
    ):
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
            self.delete(0, "end")
            self.configure(text_color=self.default_fg_color)

    def foc_out(self, *args):
        if not self.get():
            self.put_placeholder()


class LoginFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        # 配置网格为3行3列
        self.grid_rowconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)

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
            font=("Arial", 12),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        # 登录标签
        label = ctk.CTkLabel(
            master=self, text="Login Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(
            row=0, column=0, padx=20, pady=20, sticky="ew", columnspan=2, rowspan=2
        )

        # 用户ID输入框
        self.entry_userid = PlaceholderEntry(
            self, placeholder="Userid:", width=300, font=("Arial", 14)
        )
        self.entry_userid.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        # 密码输入框
        self.entry_password = PlaceholderEntry(
            self, placeholder="Password:", width=300, font=("Arial", 14)
        )
        self.entry_password.grid(
            row=2, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        # 登录按钮
        btn_login = ctk.CTkButton(
            self,
            text="Login",
            command=self.login_command,
            corner_radius=20,
            fg_color="lightgreen",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_login.grid(row=3, column=0, padx=20, pady=20, columnspan=2, sticky="n")

    def handle_login(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return

        input_data = f"su {self.entry_userid.get()} {self.entry_password.get()}\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        # 清除输入框内容
        self.entry_userid.delete(0, "end")
        self.entry_password.delete(0, "end")

        if output == "Invalid\n":
            self.show_error("Invalid Userid or Password")
        else:
            self.controller.after(0, self.controller.show_frame, FunctionFrame)

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("Error", message))

    def login_command(self):
        threading.Thread(target=self.handle_login).start()


class RegisterFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")

        self.grid_rowconfigure(0, weight=1)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_rowconfigure(4, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Register Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=0, column=0, padx=20, pady=20, columnspan=2, rowspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(InitialFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        entry_userid = PlaceholderEntry(
            self, placeholder="Userid:", width=300, font=("Arial", 14)
        )
        entry_userid.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        entry_password = PlaceholderEntry(
            self, placeholder="Password:", width=300, font=("Arial", 14)
        )
        entry_password.grid(row=2, column=0, padx=20, pady=20, columnspan=2, sticky="n")

        entry_username = PlaceholderEntry(
            self, placeholder="Username:", width=300, font=("Arial", 14)
        )
        entry_username.grid(row=3, column=0, padx=20, pady=20, columnspan=2, sticky="n")

        btn_register = ctk.CTkButton(
            self,
            text="Register",
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_register.grid(row=4, column=0, padx=20, pady=20, columnspan=2, sticky="n")

class FunctionFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_rowconfigure(4, weight=1)
        self.grid_columnconfigure(0, weight=0)
        self.grid_columnconfigure(1, weight=0)
        self.grid_columnconfigure(2, weight=0)

        label = ctk.CTkLabel(
            self, text="Function Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=0, column=0, padx=20, pady=20, columnspan=3)

        btn_back = ctk.CTkButton(
            self,
            text="Logout",
            command=self.logout_command,
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        btn_searchbook = ctk.CTkButton(
            self,
            text="Search",
            command=lambda: controller.show_frame(SearchFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_searchbook.grid(row=1, column=0, padx=0, pady=20, sticky="new")

        btn_useradd = ctk.CTkButton(
            self,
            text="Useradd",
            command=lambda: controller.show_frame(UseraddFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_useradd.grid(row=1, column=1, padx=0, pady=20, sticky="new")

        btn_passwd = ctk.CTkButton(
            self,
            text="Passwd",
            command=lambda: controller.show_frame(PasswdFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_passwd.grid(row=1, column=2, padx=0, pady=20, sticky="new")

        btn_delete_command = ctk.CTkButton(
            self,
            text="Delete",
            command=lambda: controller.show_frame(DeleteFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_delete_command.grid(row=2, column=0, padx=0, pady=20, sticky="new")

        btn_select_command = ctk.CTkButton(
            self,
            text="Select",
            command=lambda: controller.show_frame(SelectFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_select_command.grid(row=2, column=1, padx=0, pady=20, sticky="new")

        btn_modify_command = ctk.CTkButton(
            self,
            text="Modify",
            command=lambda: controller.show_frame(ModifyFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_modify_command.grid(row=2, column=2, padx=0, pady=20, sticky="new")

        btn_log_command = ctk.CTkButton(
            self,
            text="Log",
            command=self.log_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_log_command.grid(row=3, column=0, padx=0, pady=20, sticky="ne")

        btn_report_finance = ctk.CTkButton(
            self,
            text="Report Finance",
            command=self.report_finance_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_report_finance.grid(row=3, column=1, padx=0, pady=20, sticky="new")

        btn_report_employee = ctk.CTkButton(
            self,
            text="Report Employee",
            command=self.report_employee_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_report_employee.grid(row=3, column=2, padx=0, pady=20, sticky="new")

        btn_import = ctk.CTkButton(
            self,
            text="Import",
            command=lambda: controller.show_frame(ImportFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_import.grid(row=4, column=0, padx=0, pady=20, sticky="new")

        btn_buy = ctk.CTkButton(
            self,
            text="Buy",
            command=lambda: controller.show_frame(BuyFrame),
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=200,
            height=50,
            font=("Arial", 14),
        )
        btn_buy.grid(row=4, column=1, padx=0, pady=20, sticky="new")

    

    def handle_employee_command(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return
        input_data = f"report employee\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output_lines = []
            while True:
                line = process.stdout.readline()
                if line == "Invalid\n":
                    self.show_error("Invalid Report")
                    return
                if line == "Report success.\n":
                    break
                output_lines.append(line)
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        ResultFrame.result = "".join(output_lines)
        self.controller.frames[ResultFrame].show_result()
        self.controller.after(0, self.controller.show_frame, ResultFrame)

    def report_employee_command(self):
        threading.Thread(target=self.handle_employee_command).start()
        
    def handle_report_finance(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return
        input_data = f"report finance\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output_lines = []
            while True:
                line = process.stdout.readline()
                if line == "Invalid\n":
                    self.show_error("Invalid Report")
                    return
                if line == "Report success.\n":
                    break
                output_lines.append(line)
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        ResultFrame.result = "".join(output_lines)
        self.controller.frames[ResultFrame].show_result()
        self.controller.after(0, self.controller.show_frame, ResultFrame)

    def report_finance_command(self):
        threading.Thread(target=self.handle_report_finance).start()

    

    def handle_log(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return
        input_data = f"log\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output_lines = []
            while True:
                line = process.stdout.readline()
                if line == "Invalid\n":
                    self.show_error("Invalid Log")
                    return
                if line == "Log success.\n":
                    break
                output_lines.append(line)
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        ResultFrame.result = "".join(output_lines)
        self.controller.frames[ResultFrame].show_result()
        self.controller.after(0, self.controller.show_frame, ResultFrame)

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("LogError", message))

    def log_command(self):
        threading.Thread(target=self.handle_log).start()
    
    def logout_command(self):
        global process
        global is_open
        if is_open:
            try:
                process.stdin.write("exit\n")
                process.stdin.flush()
                process.terminate()
                is_open = False
            except Exception as e:
                print(f"退出时出错: {e}")
        self.controller.show_frame(InitialFrame)

class SearchFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure((1, 2, 3, 4, 5, 6, 7), weight=0)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Search Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=0, column=0, padx=20, pady=20, columnspan=2, rowspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        self.entry_search = PlaceholderEntry(
            self, placeholder="Search:", width=300, font=("Arial", 14)
        )
        self.entry_search.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        btn_search = ctk.CTkButton(
            self,
            text="Search",
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            command=self.search_command,
            font=("Arial", 14),
        )
        btn_search.grid(
            row=7, column=0, padx=20, pady=20, columnspan=2, sticky="s", rowspan=2
        )

        self.radio_var = tkinter.StringVar(value="All")
        
        self.radioAll = ctk.CTkRadioButton(
            self,
            text="All",
            variable=self.radio_var,
            value="All",
            command=self.radio_event,
        )
        self.radioAll.grid(row=2, column=0, padx=20, pady=10, columnspan=2)

        self.radioISBN = ctk.CTkRadioButton(
            self,
            text="ISBN",
            variable=self.radio_var,
            value="-ISBN=",
            command=self.radio_event,
        )
        self.radioISBN.grid(row=3, column=0, padx=20, pady=10, columnspan=2)

        self.radioname = ctk.CTkRadioButton(
            self,
            text="Name",
            variable=self.radio_var,
            value="-name=",
            command=self.radio_event,
        )
        self.radioname.grid(row=4, column=0, padx=20, pady=10, columnspan=2)

        self.radioauthor = ctk.CTkRadioButton(
            self,
            text="Author",
            variable=self.radio_var,
            value="-author=",
            command=self.radio_event,
        )
        self.radioauthor.grid(row=5, column=0, padx=20, pady=10, columnspan=2)

        self.radiokeyword = ctk.CTkRadioButton(
            self,
            text="Keyword",
            variable=self.radio_var,
            value="-keyword=",
            command=self.radio_event,
        )
        self.radiokeyword.grid(row=6, column=0, padx=20, pady=10, columnspan=2)

    def radio_event(self):
        currentchoice=self.radio_var.get()
        if currentchoice == "All":
            self.entry_search.grid_remove()
        else:
            self.entry_search.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

    def handle_search(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return
        if self.radio_var.get() == "All":
            input_data = f"show\n"
        elif self.radio_var.get() == "-ISBN=":
            input_data = f"show {self.radio_var.get()}{self.entry_search.get()}\n"
        else :
            input_data = f"show {self.radio_var.get()}\"{self.entry_search.get()}\"\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output_lines = []
            while True:
                line = process.stdout.readline()
                if line == "Invalid\n":
                    self.entry_search.delete(0, "end")
                    self.show_error("Invalid Search")
                    return
                if line == "show success\n":
                    break
                output_lines.append(line)
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        # 清除输入框内容
        self.entry_search.delete(0, "end")
        ResultFrame.result = "".join(output_lines)
        self.controller.frames[ResultFrame].show_result()
        self.controller.after(0, self.controller.show_frame, ResultFrame)

    def search_command(self):
        threading.Thread(target=self.handle_search).start()

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("SearchError", message))
    

class ResultFrame(ctk.CTkFrame):
    result = ""

    def __init__(self, parent, controller):
        super().__init__(parent)
        self.controller = controller

        # 创建文本显示区域和滚动条
        self.text_area = ctk.CTkTextbox(self, width=500, height=300, font=("Arial", 14))
        self.text_area.grid(row=1, column=0, padx=20, pady=20, sticky="nsew")

        # 返回按钮
        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

    def show_result(self):
        self.text_area.delete("1.0", "end")
        self.text_area.insert("1.0", self.result)


class UseraddFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure((1, 2, 3, 4, 5, 6), weight=0)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Useradd Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=0, column=0, padx=20, pady=20, columnspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        self.entry_userid = PlaceholderEntry(
            self, placeholder="Userid:", width=300, font=("Arial", 14)
        )
        self.entry_userid.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        self.entry_userpassword = PlaceholderEntry(
            self, placeholder="Password:", width=300, font=("Arial", 14)
        )
        self.entry_userpassword.grid(
            row=2, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        self.entry_userprivilege = PlaceholderEntry(
            self, placeholder="Privilege:", width=300, font=("Arial", 14)
        )
        self.entry_userprivilege.grid(
            row=3, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        self.entry_username = PlaceholderEntry(
            self, placeholder="Username:", width=300, font=("Arial", 14)
        )
        self.entry_username.grid(
            row=4, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        btn_useradd = ctk.CTkButton(
            self,
            text="Useradd",
            command=self.useradd_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_useradd.grid(row=5, column=0, padx=20, pady=20, columnspan=2, sticky="n")

    def handle_useradd(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return

        input_data = f"useradd {self.entry_userid.get()} {self.entry_userpassword.get()} {self.entry_userprivilege.get()} {self.entry_username.get()}\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        self.entry_userid.delete(0, "end")
        self.entry_userpassword.delete(0, "end")
        self.entry_userprivilege.delete(0, "end")
        self.entry_username.delete(0, "end")
        if output == "Invalid\n":
            self.show_error("Invalid Userid or Password")
        else:
            self.show_success()
            self.controller.after(0, self.controller.show_frame, FunctionFrame)

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("UseraddError", message))

    def useradd_command(self):
        threading.Thread(target=self.handle_useradd).start()

    def show_success(self):
        self.controller.after(
            0, lambda: messagebox.showinfo("UseraddSuccess", "Useradd Success")
        )


class DeleteFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_rowconfigure(4, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Delete Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        self.entry_userid = PlaceholderEntry(
            self, placeholder="Userid:", width=300, font=("Arial", 14)
        )
        self.entry_userid.grid(row=2, column=0, padx=20, pady=20, columnspan=2)

        btn_delete = ctk.CTkButton(
            self,
            text="Delete",
            command=self.delete_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_delete.grid(row=3, column=0, padx=20, pady=20, columnspan=2, sticky="n")

    def handle_delete(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return

        input_data = f"delete {self.entry_userid.get()}\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        self.entry_userid.delete(0, "end")
        if output == "Invalid\n":
            self.show_error("Invalid Userid")
        else:
            self.show_success()
            self.controller.after(0, self.controller.show_frame, FunctionFrame)

    def delete_command(self):
        threading.Thread(target=self.handle_delete).start()

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("DeleteError", message))

    def show_success(self):
        self.controller.after(
            0, lambda: messagebox.showinfo("DeleteSuccess", "Delete Success")
        )


class PasswdFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_rowconfigure(4, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Passwd Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=0, column=0, padx=20, pady=20, columnspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        self.entry_userid = PlaceholderEntry(
            self, placeholder="Userid:", width=300, font=("Arial", 14)
        )
        self.entry_userid.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        self.entry_currentpasswd = PlaceholderEntry(
            self, placeholder="Current Password:", width=300, font=("Arial", 14)
        )
        self.entry_currentpasswd.grid(row=2, column=0, padx=20, pady=20, columnspan=2)

        self.entry_newpasswd = PlaceholderEntry(
            self, placeholder="New Password:", width=300, font=("Arial", 14)
        )
        self.entry_newpasswd.grid(
            row=3, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        btn_passwd = ctk.CTkButton(
            self,
            text="Passwd",
            command=self.passwd_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_passwd.grid(row=4, column=0, padx=20, pady=20, columnspan=2, sticky="n")

    def handle_passwd(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return

        input_data = f"passwd {self.entry_userid.get()} {self.entry_currentpasswd.get()} {self.entry_newpasswd.get()}\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        self.entry_userid.delete(0, "end")
        self.entry_currentpasswd.delete(0, "end")
        self.entry_newpasswd.delete(0, "end")

        if output == "Invalid\n":
            self.show_error("Invalid Userid or Password")
        else:
            self.show_success()
            self.controller.after(0, self.controller.show_frame, FunctionFrame)

    def passwd_command(self):
        threading.Thread(target=self.handle_passwd).start()

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("PasswdError", message))

    def show_success(self):
        self.controller.after(
            0, lambda: messagebox.showinfo("PasswdSuccess", "Passwd Success")
        )


class SelectFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_rowconfigure(4, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Select Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        self.entry_bookid = PlaceholderEntry(
            self, placeholder="Bookid:", width=300, font=("Arial", 14)
        )
        self.entry_bookid.grid(
            row=2, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        btn_select = ctk.CTkButton(
            self,
            text="Select",
            command=self.select_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_select.grid(row=3, column=0, padx=20, pady=20, columnspan=2, sticky="n")

    def handle_select(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return

        input_data = f"select {self.entry_bookid.get()}\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        self.entry_bookid.delete(0, "end")
        if output == "Invalid\n":
            self.show_error("Invalid Bookid")
        else:
            self.show_success()
            self.controller.after(0, self.controller.show_frame, FunctionFrame)

    def select_command(self):
        threading.Thread(target=self.handle_select).start()

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("SelectError", message))

    def show_success(self):
        self.controller.after(
            0, lambda: messagebox.showinfo("SelectSuccess", "Select Success")
        )

class ModifyFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent)
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure((1,2,3,4,5,6), weight=0)
        self.grid_columnconfigure(0, weight=0)
        self.grid_columnconfigure(1, weight=1)

        label = ctk.CTkLabel(
            self, text="Modify Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        btn_modify = ctk.CTkButton(
            self,
            text="Modify",
            command=self.modify_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_modify.grid(row=6, column=0, padx=20, pady=20, columnspan=2, sticky="n")

        self.check_vars = {
            'isbn': tkinter.BooleanVar(value=False),
            'name': tkinter.BooleanVar(value=False),
            'author': tkinter.BooleanVar(value=False),
            'keyword': tkinter.BooleanVar(value=False)
        }
        
        self.entries = {
            'isbn': PlaceholderEntry(self, placeholder="ISBN:", width=300, font=("Arial", 14)),
            'name': PlaceholderEntry(self, placeholder="Name:", width=300, font=("Arial", 14)),
            'author': PlaceholderEntry(self, placeholder="Author:", width=300, font=("Arial", 14)),
            'keyword': PlaceholderEntry(self, placeholder="Keyword:", width=300, font=("Arial", 14))
        }
        
        self.checkbox_isbn = ctk.CTkCheckBox(
            self,
            text="ISBN",
            variable=self.check_vars['isbn'],
            command=lambda: self.on_checkbox_select('isbn')
        )
        self.checkbox_isbn.grid(row=2, column=0, padx=20, pady=10,columnspan=2, sticky="nw")
        
        self.checkbox_name = ctk.CTkCheckBox(
            self,
            text="Name",
            variable=self.check_vars['name'],
            command=lambda: self.on_checkbox_select('name')
        )
        self.checkbox_name.grid(row=3, column=0, padx=20, pady=10,columnspan=2, sticky="nw")
        
        self.checkbox_author = ctk.CTkCheckBox(
            self,
            text="Author",
            variable=self.check_vars['author'],
            command=lambda: self.on_checkbox_select('author')
        )
        self.checkbox_author.grid(row=4, column=0, padx=20, pady=10,columnspan=2, sticky="nw")
        
        self.checkbox_keyword = ctk.CTkCheckBox(
            self,
            text="Keyword",
            variable=self.check_vars['keyword'],
            command=lambda: self.on_checkbox_select('keyword')
        )
        self.checkbox_keyword.grid(row=5, column=0, padx=20, pady=(10,100),columnspan=2, sticky="nw")

    def on_checkbox_select(self, key):
        # 如果选中则显示对应输入框，否则隐藏
        if self.check_vars[key].get():
            self.entries[key].grid(row={'isbn':2, 'name':3, 'author':4, 'keyword':5}[key], 
                                 column=1, padx=20, pady=10,sticky="ne")
        else:
            self.entries[key].grid_remove()

    def handle_modify(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  # 以文本模式处理输入输出
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return

        input_data = "modify"
        for key in self.check_vars:
            if self.check_vars[key].get():
                if key=='isbn':
                    input_data += f" -ISBN={self.entries[key].get()}"
                elif key=='name':
                    input_data += f" -name=\"{self.entries[key].get()}\""
                elif key=='author':
                    input_data += f" -author=\"{self.entries[key].get()}\""
                elif key=='keyword':
                    input_data += f" -keyword=\"{self.entries[key].get()}\""
        input_data += "\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return

        for key in self.check_vars:
            self.entries[key].delete(0, "end")
        if output == "Invalid\n":
            self.show_error("Invalid Modify")
        else:
            self.show_success()
            self.controller.after(0, self.controller.show_frame, FunctionFrame)
        
    def modify_command(self):
        threading.Thread(target=self.handle_modify).start()

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("ModifyError", message))

    def show_success(self):
        self.controller.after(
            0, lambda: messagebox.showinfo("ModifySuccess", "Modify Success")
        )

class ImportFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_rowconfigure(4, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Import Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        self.entry_quantity = PlaceholderEntry(
            self, placeholder="Quantity:", width=300, font=("Arial", 14)
        )
        self.entry_quantity.grid(row=2, column=0, padx=20, pady=20, columnspan=2)

        self.entry_totalprice = PlaceholderEntry(
            self, placeholder="TotalPrice:", width=300, font=("Arial", 14)
        )
        self.entry_totalprice.grid(row=3, column=0, padx=20, pady=20, columnspan=2)

        btn_import = ctk.CTkButton(
            self,
            text="Import",
            command=self.import_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_import.grid(row=4, column=0, padx=20, pady=20, columnspan=2, sticky="n")

    def handle_import(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return
    
        input_data = f"import {self.entry_quantity.get()} {self.entry_totalprice.get()}\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return
        
        self.entry_quantity.delete(0, "end")
        self.entry_totalprice.delete(0, "end")
        if output == "Invalid\n":
            self.show_error("Invalid Filename")
        else:
            self.show_success()
            self.controller.after(0, self.controller.show_frame, FunctionFrame)

    def import_command(self):
        threading.Thread(target=self.handle_import).start()

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("ImportError", message))

    def show_success(self):
        self.controller.after(
            0, lambda: messagebox.showinfo("ImportSuccess", "Import Success")
        )

class BuyFrame(ctk.CTkFrame):
    def __init__(self, parent, controller):
        super().__init__(parent, fg_color="white")
        self.controller = controller

        self.grid_rowconfigure(0, weight=0)
        self.grid_rowconfigure(1, weight=0)
        self.grid_rowconfigure(2, weight=1)
        self.grid_rowconfigure(3, weight=1)
        self.grid_rowconfigure(4, weight=1)
        self.grid_columnconfigure(0, weight=1)
        self.grid_columnconfigure(1, weight=0)

        label = ctk.CTkLabel(
            self, text="Buy Surface", font=("Monaco", 25), text_color="black"
        )
        label.grid(row=1, column=0, padx=20, pady=20, columnspan=2)

        btn_back = ctk.CTkButton(
            self,
            text="Return",
            command=lambda: controller.show_frame(FunctionFrame),
            corner_radius=20,
            fg_color="lightgray",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_back.grid(row=0, column=0, padx=20, pady=20, sticky="nw")

        self.entry_bookid = PlaceholderEntry(
            self, placeholder="ISBN:", width=300, font=("Arial", 14)
        )
        self.entry_bookid.grid(
            row=2, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        self.entry_number = PlaceholderEntry(
            self, placeholder="Number:", width=300, font=("Arial", 14)
        )
        self.entry_number.grid(
            row=3, column=0, padx=20, pady=20, columnspan=2, sticky="n"
        )

        btn_buy = ctk.CTkButton(
            self,
            text="Buy",
            command=self.buy_command,
            corner_radius=20,
            fg_color="lightblue",
            text_color="black",
            width=100,
            height=40,
            font=("Arial", 14),
        )
        btn_buy.grid(row=4, column=0, padx=20, pady=20, columnspan=2, sticky="n")

    def handle_buy(self):
        global process
        global is_open
        try:
            if not is_open:
                command = "/Users/apple/Desktop/Programming/ACMOJ/Bookstore/Bookstore-2024/build/code"
                process = subprocess.Popen(
                    command,
                    stdin=subprocess.PIPE,
                    stdout=subprocess.PIPE,
                    stderr=subprocess.PIPE,
                    shell=True,
                    universal_newlines=True,  
                )
                is_open = True
                atexit.register(exit)
        except Exception as e:
            self.show_error(f"启动子进程失败: {e}")
            return
        
        input_data = f"buy {self.entry_bookid.get()} {self.entry_number.get()}\n"
        try:
            process.stdin.write(input_data)
            process.stdin.flush()
            output = process.stdout.readline()
        except Exception as e:
            self.show_error(f"通信失败: {e}")
            return
        
        self.entry_bookid.delete(0, "end")
        self.entry_number.delete(0, "end")
        if output == "Invalid\n":
            self.show_error("Invalid Bookid or Number")
        else:
            self.controller.after(
                0, lambda: messagebox.showinfo("BuySuccess", "TotalPrice: "+output)
            )
            self.controller.after(0, self.controller.show_frame, FunctionFrame)
        
    def buy_command(self):
        threading.Thread(target=self.handle_buy).start()

    def show_error(self, message):
        self.controller.after(0, lambda: messagebox.showinfo("BuyError", message))
      

if __name__ == "__main__":
    os.chdir("/Users/apple/Desktop/Programming/ACMOJ/BookStore/Bookstore-2024/build")
    app = App()
    app.mainloop()
