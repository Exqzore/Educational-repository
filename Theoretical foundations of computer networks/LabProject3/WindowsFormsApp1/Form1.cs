using System;
using System.Drawing;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void InputBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = (e.KeyChar != '0' && e.KeyChar != '1' && e.KeyChar != 8) || inputBox.TextLength == 32;

            if (e.KeyChar == 13)
            {
                if (inputBox.TextLength < 32)
                    while (inputBox.TextLength != 32)
                        inputBox.AppendText("0");
                string tmp = inputBox.Text;
                tmp = Coder(tmp);
                statusBox.Text = "Serving length: 32\nInput data after coding:" + Environment.NewLine + tmp + Environment.NewLine;
                //tmp = tmp.Remove(13, 1).Insert(13, "0");
                tmp = Decoder(tmp);
                outputBox.Text = tmp;
                SelectionBits(44, statusBox.Text.Substring(44, 38));
            }
        }

        private string Decoder(string str)
        {
            string oldStr = str;
            int i = 0, index, pos = 0, flag = 0;
            while (Math.Pow(2, i) - 1 < str.Length) i++;
            for (--i ; i >= 0; i--) 
                str = str.Remove((int)Math.Pow(2, i) - 1, 1);
            str = Coder(str);
            i = 0;
            while ((index = (int)Math.Pow(2, i++) - 1) < str.Length)
                if (oldStr[index] != str[index])
                {
                    pos += index + 1;
                    flag = 1;
                }
            if(flag != 0) str = str.Insert(pos, str[pos] == '0' ? "1" : "0").Remove(pos + 1, 1);
            i = 0;
            while (Math.Pow(2, i) - 1 < str.Length) i++;
            for (--i; i >= 0; i--)
                str = str.Remove((int)Math.Pow(2, i) - 1, 1);
            return str;
        }

        private string Coder(string str)
        {
            int index, i = 0;
            while ((index = (int)Math.Pow(2, i++) - 1) < str.Length) 
                str = str.Insert(index, "0");
            i = 0;
            while ((index = (int)Math.Pow(2, i++) - 1) < str.Length)
            {
                int j = index, count = 0;
                while (true)
                    if (j < str.Length)
                    {
                        count += CountOne(str.Substring(j, index + 1 <= str.Length - j ? index + 1 : str.Length - j));
                        j += 2 * (index + 1);
                    }
                    else break;
                if (count % 2 != 0) str = str.Remove(index, 1).Insert(index, "1");
            }
            return str;
        }

        private int CountOne(string str)
        {
            int count = 0;
            foreach (char c in str)
                if (c == '1') count++;
            return count;
        }

        private void SelectionBits(int startPosition, string str)
        {
            int index, i = 0;
            while ((index = (int)Math.Pow(2, i++) - 1) < str.Length)
            {
                statusBox.SelectionStart = startPosition + index;
                statusBox.SelectionLength = 1;
                statusBox.SelectionColor = Color.Red;
            }
        }
    }
}