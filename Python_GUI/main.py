import sys
import glob
import serial
from time import sleep
try:
    # python 3.x
    import tkinter as tk
    import tkinter.ttk as ttk
except ImportError:
    # python 2.x
    import Tkinter as tk
    import ttk


def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError: On unsupported or unknown platforms

        :returns: list of serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # excludes current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            ser = serial.Serial(port)
            ser.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result


class CustomCombobox(tk.Frame):
    def __init__(self, parent, values: list, label: str = "", current: int = 0):
        """ Configures new combobox and label widgets

            :param label: label text
            :param values: combobox content list
            :param current: index of default value

            :return: None
        """
        tk.Frame.__init__(self, parent)

        self.label = tk.Label(self, text=label, anchor="w")
        self.combo = ttk.Combobox(self, values=values)
        self.combo.current(current)

        self.label.grid(column=0, row=0)
        self.combo.grid(column=0, row=1)

    def get(self):
        """
            :return: value of combobox
        """
        return self.combo.get()


class CustomSpinbox(tk.Frame):
    def __init__(self, parent, label: str = "", from_: int = 1, to: int = 100, increment: int = 1):
        """ Configures new spinbox and label widgets

            :param label: label text
            :param from_: min value in spinbox
            :param to: max value in spinbox
            :param increment: spinbox single step length

            :return: None
        """
        tk.Frame.__init__(self, parent)

        self.label = tk.Label(self, text=label, anchor="w")
        self.spin = tk.Spinbox(self, from_=from_, to=to, increment=increment)

        self.label.grid(column=0, row=0)
        self.spin.grid(column=0, row=1)

    def get(self):
        """
            :return: value of spinbox
        """
        return self.spin.get()


class CustomButton(tk.Frame):
    def __init__(self, parent, command: object, text: str = ""):
        """ Configures new button widgets

            :param command: function on click event
            :param text: button text

            :return: None
        """
        tk.Frame.__init__(self, parent)

        self.btn = tk.Button(self, text=text, command=command)

        self.btn.grid(column=0, row=0, pady=4)


class ConfigWindow(tk.Frame):
    def __init__(self, parent):
        """ Creates config window

            :return: None
        """
        tk.Frame.__init__(self, parent)
        self.port = CustomCombobox(self, serial_ports(), "Port", 0)
        self.colors = CustomCombobox(self, [1, 2, 3, 4], "Farben", 0)
        self.simon = CustomSpinbox(self, "Simon Says", 1, 255, 1)
        self.asteroids = CustomSpinbox(self, "Asteroiden", 1, 255, 1)
        self.submitBtn = CustomButton(self, self.submit, "Daten Senden")

        padx = 4
        pady = 4
        self.port.grid(row=0, column=0, padx=padx, pady=pady)
        self.colors.grid(row=1, column=0, padx=padx, pady=pady)
        self.simon.grid(row=2, column=0, padx=padx, pady=pady)
        self.asteroids.grid(row=3, column=0, padx=padx, pady=pady)
        self.submitBtn.grid(row=4, column=0, padx=padx, pady=pady)

    def submit(self):
        """ Sends settings configured in GUI to STM32 via UART

            :return: None
        """
        port = self.port.get()
        ser = serial.Serial(port, 115200, timeout=2)
        if ser.isOpen():
            print("Serial port open")

            id_simon = 1
            data_simon = int(self.simon.get())
            data_simon = (id_simon & 0xff) | (data_simon << 8);
            ser.write(data_simon.to_bytes(2, "little"))

            id_colors = 2
            data_colors = int(self.colors.get())
            data_colors = (id_colors & 0xff) | (data_colors << 8);
            ser.write(data_colors.to_bytes(2, "little"))

            id_asteroids = 3
            data_asteroids = int(self.asteroids.get())
            data_asteroids = (id_asteroids & 0xff) | (data_asteroids << 8);
            ser.write(data_asteroids.to_bytes(2, "little"))

        else:
            print("Serial port not open")
        ser.close()
        if ser.isOpen():
            print("Serial port not closed")
        else:
            print("Serial port closed")


if __name__ == "__main__":
    root = tk.Tk()
    root.title("Rätselbox")
    root.geometry('150x240')
    ConfigWindow(root).place(x=0, y=0, relwidth=1, relheight=1)
    root.mainloop()
