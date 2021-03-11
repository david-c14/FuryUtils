﻿using System;
using System.Windows.Forms;
using System.IO;
using carbon14.FuryUtils;

namespace TestPlatform
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Button1_Click(object sender, EventArgs e)
        {
            byte[] buffer;
            using (FileStream fs = new FileStream("LANG.DAT", FileMode.Open))
            {
                buffer = new byte[fs.Length];
                fs.Read(buffer, 0, buffer.Length);
            }
            DatFile df = new DatFile(buffer);
            MessageBox.Show(df.EntryCount.ToString());
            foreach (DatFile.DatFileEntry dfe in df.Entries())
            {
                MessageBox.Show(dfe.FileName);
                if (dfe.FileName == "PASSW.DTA")
                {
                    byte[] passwordFile = dfe.Buffer();

                }
            }

        }

        private void Button2_Click(object sender, EventArgs e)
        {
            DatFile df = new DatFile();
            byte[] buffer;
            using (FileStream fs = new FileStream("LANG.TXT", FileMode.Open))
            {
                buffer = new byte[fs.Length];
                fs.Read(buffer, 0, buffer.Length);
            }
            df.Add("LANG2.TXT", buffer, true);
            using (FileStream fs = new FileStream("PASSW.DTA", FileMode.Open))
            {
                buffer = new byte[fs.Length];
                fs.Read(buffer, 0, buffer.Length);
            }
            df.Add("PASSW2.DTA", buffer, true);
            buffer = df.Buffer();

            MessageBox.Show(df.EntryCount.ToString());
            foreach(DatFile.DatFileEntry dfe in df.Entries())
            {
                MessageBox.Show(dfe.FileName);
            }
        }
    }
}