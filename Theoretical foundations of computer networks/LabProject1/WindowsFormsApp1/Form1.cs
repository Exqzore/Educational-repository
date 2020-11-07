using System;
using System.IO.Ports;
using System.Linq;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        SerialPort serialPortCom1;
        SerialPort serialPortCom2;

        public Form1()
        {           
            InitializeComponent();
            try
            {
                serialPortCom1 = new SerialPort("COM1", 9600, Parity.None, 8, StopBits.One);
                serialPortCom1.Open();

                serialPortCom2 = new SerialPort("COM2", 9600, Parity.None, 8, StopBits.One);
                serialPortCom2.DataReceived += new SerialDataReceivedEventHandler(SerialPort_DataReceived);
                serialPortCom2.Open();

                PortInfoOutput(serialPortCom1);
                PortInfoOutput(serialPortCom2);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString(), "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                this.Close();
            }           
        }

        void PortInfoOutput(SerialPort port)
        {
            textBox3.Text += "Port name: " + port.PortName + Environment.NewLine
                + "Parity: " + port.Parity + Environment.NewLine
                + "StopBits: " + port.StopBits + Environment.NewLine
                + "DataBits: " + port.DataBits + Environment.NewLine
                + "Write Timeout: " + (port.WriteTimeout == -1 ? "infinit" : port.WriteTimeout.ToString()) + Environment.NewLine
                + "Ride Timeout: " + (port.ReadTimeout == -1 ? "infinit" : port.ReadTimeout.ToString()) + Environment.NewLine
                + "BaudRate: " + port.BaudRate + Environment.NewLine + Environment.NewLine;
        }

        void SerialPort_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            string tmp = serialPortCom2.ReadExisting();
            textBox2.AppendText(tmp.Last() == '\n' ? Environment.NewLine : tmp);
        }

        private void TextBox1_TextChanged(object sender, EventArgs e)
        {
            if (textBox1.Lines.Length + textBox1.Text.Length / 92 == 9)
            {
                textBox1.ScrollBars = ScrollBars.Vertical;
                textBox2.ScrollBars = ScrollBars.Vertical;
            }
            serialPortCom1.Write(textBox1.Text.Last().ToString());
        }

        private void TextBox1_KeyPress(object sender, KeyPressEventArgs e)
        {
            if ((e.KeyChar >= 'А' && e.KeyChar <= 'Я') || (e.KeyChar >= 'а' && e.KeyChar <= 'я') || e.KeyChar == (char)Keys.Back) e.Handled = true;
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            serialPortCom1.Close();
            serialPortCom2.Close();
        }

        private void TextBox1_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            textBox1.Focus();
            textBox1.SelectionStart = textBox1.Text.Length;
        }
    }
}
