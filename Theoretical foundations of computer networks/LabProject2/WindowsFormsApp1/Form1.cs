using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        private string Flag { get; set; } = "01111011";
        private List<int> listPosFlagBit;

        public Form1()
        {
            InitializeComponent();
            listPosFlagBit = new List<int>();
            statusBox.Text = "Flag:" + Flag + Environment.NewLine;
        }

        private void InputBox_KeyPress(object sender, KeyPressEventArgs e)
        {
            e.Handled = (e.KeyChar != '0' && e.KeyChar != '1' && e.KeyChar != 8) || inputBox.TextLength == 38;

            if (e.KeyChar == 13)
            {
                if (inputBox.TextLength != 0)
                {
                    statusBox.Text = "Flag:" + Flag + Environment.NewLine;
                    string tmp = BitStuffing(inputBox.Text);
                    statusBox.Text += tmp;
                    SelectionBits();
                    outputBox.Text = DebitStuffing(tmp);
                }
                else statusBox.Text = "Flag:" + Flag + Environment.NewLine;
            }
        }

        string BitStuffing(string data)
        {
            listPosFlagBit.Clear();
            int i, j = 0;
            while ((i = data.Substring(j, data.Length - j).IndexOf("0111101")) != -1)
            {
                data = data.Insert(i + j + 7, "0");
                listPosFlagBit.Add(i + j + 7);
                j += i + 1;
            }
            return data;
        }

        string DebitStuffing(string data)
        {
            int i = data.Length - 6;
            while ((i = data.Substring(0, i + 6).LastIndexOf("0111101")) != -1) 
                data = data.Remove(i + 7, 1);
            return data;
        }

        private void SelectionBits()
        {
            for (int i = 0; i < listPosFlagBit.Count; i++)
            {
                statusBox.SelectionStart = listPosFlagBit[i] + 14;
                statusBox.SelectionLength = 1;
                statusBox.SelectionColor = Color.Red;
            }
        }
    }
}