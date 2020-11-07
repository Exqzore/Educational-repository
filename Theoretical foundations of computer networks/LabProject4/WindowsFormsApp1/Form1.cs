using System.Windows.Forms;
using System.Threading;
using System;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        private bool sending = false;

        public Form1()
        {
            InitializeComponent();
        }

        private void InputBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = inputBox.TextLength == 40 || sending;
        }

        private void SendSymbol(char c)
        {
            var randomValue = new Random();
            sending = true;
            int tryCount = 0;
            statusBox.AppendText(c + ":");
            while (true)
            {
                if (randomValue.Next(1, 4) != 1) break;
                Thread.Sleep(300);
            }
            while (true)
            {
                Thread.Sleep(500);
                if (randomValue.Next(1, 4) == 1)
                {
                    statusBox.AppendText("*");
                    tryCount++;
                    if (tryCount == 10)
                    {
                        statusBox.AppendText("\n"/*", Symbol transmission error\n"*/);
                        statusBox.ScrollToCaret();
                        sending = false;
                        return;
                    }
                    Thread.Sleep((int)(0.1 / 500 * randomValue.Next(0, (int)Math.Pow(2, tryCount))));
                }
                else
                {
                    outputBox.AppendText(c.ToString());
                    statusBox.AppendText("\n"/*tryCount == 0 ? "Symbol passed\n" : ", Symbol passed\n"*/);
                    statusBox.ScrollToCaret();
                    sending = false;
                    return;
                }
            }
        }

        private void InputBox_TextChanged(object sender, EventArgs e)
        {
            if (inputBox.TextLength != 0)
            {
                new Thread(() =>
                {
                    SendSymbol(inputBox.Text[inputBox.TextLength - 1]);
                })
                {
                    IsBackground = false
                }.Start();
            }
        }

        private void InputBox_PreviewKeyDown(object sender, PreviewKeyDownEventArgs e)
        {
            if (e.KeyCode.ToString() == "Back")
            {
                inputBox.Clear();
                outputBox.Clear();
                statusBox.Clear();
            }
            inputBox.SelectionStart = inputBox.Text.Length;
        }
    }
}