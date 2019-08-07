namespace SoftPhoneCSharp
{
    partial class frmSetting
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            LightUI.ImageRect imageRect17 = new LightUI.ImageRect();
            LightUI.GradientColor gradientColor2 = new LightUI.GradientColor();
            LightUI.ImageRect imageRect18 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect19 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect20 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect21 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect22 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect23 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect24 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect25 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect26 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect27 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect28 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect29 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect30 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect31 = new LightUI.ImageRect();
            LightUI.ImageRect imageRect32 = new LightUI.ImageRect();
            this.plFill = new System.Windows.Forms.Panel();
            this.plLeft = new LightUI.PanelEx();
            this.rdbVoiceSet = new LightUI.RadioButtonEx();
            this.rdbVideoSet = new LightUI.RadioButtonEx();
            this.rdbAccount = new LightUI.RadioButtonEx();
            this.plLeft.SuspendLayout();
            this.SuspendLayout();
            // 
            // plFill
            // 
            this.plFill.Dock = System.Windows.Forms.DockStyle.Fill;
            this.plFill.Location = new System.Drawing.Point(270, 0);
            this.plFill.Name = "plFill";
            this.plFill.Padding = new System.Windows.Forms.Padding(2);
            this.plFill.Size = new System.Drawing.Size(729, 628);
            this.plFill.TabIndex = 3;
            this.plFill.Paint += new System.Windows.Forms.PaintEventHandler(this.plFill_Paint);
            // 
            // plLeft
            // 
            this.plLeft.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(240)))), ((int)(((byte)(240)))));
            imageRect17.Corner = new System.Windows.Forms.Padding(0);
            imageRect17.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect17.Image = null;
            imageRect17.Source = new System.Drawing.Rectangle(0, 0, 0, 0);
            this.plLeft.BackgroundBitmap = imageRect17;
            this.plLeft.BorderColor = System.Drawing.Color.Red;
            this.plLeft.Controls.Add(this.rdbVoiceSet);
            this.plLeft.Controls.Add(this.rdbVideoSet);
            this.plLeft.Controls.Add(this.rdbAccount);
            this.plLeft.Dock = System.Windows.Forms.DockStyle.Left;
            gradientColor2.BackColor1 = System.Drawing.Color.FromArgb(((int)(((byte)(224)))), ((int)(((byte)(237)))), ((int)(((byte)(255)))));
            gradientColor2.BackColor2 = System.Drawing.Color.FromArgb(((int)(((byte)(178)))), ((int)(((byte)(212)))), ((int)(((byte)(255)))));
            gradientColor2.Enable = false;
            gradientColor2.GradientAngle = 90F;
            this.plLeft.GradientColor = gradientColor2;
            this.plLeft.Location = new System.Drawing.Point(0, 0);
            this.plLeft.MouseMsgSendtoForm = false;
            this.plLeft.Name = "plLeft";
            this.plLeft.ShowBorder = false;
            this.plLeft.Size = new System.Drawing.Size(270, 628);
            this.plLeft.TabIndex = 2;
            this.plLeft.Paint += new System.Windows.Forms.PaintEventHandler(this.plLeft_Paint);
            // 
            // rdbVoiceSet
            // 
            this.rdbVoiceSet.BackColor = System.Drawing.SystemColors.Control;
            imageRect18.Corner = new System.Windows.Forms.Padding(0);
            imageRect18.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect18.Image = null;
            imageRect18.Source = new System.Drawing.Rectangle(0, 0, 0, 0);
            this.rdbVoiceSet.BackgroundDisabled = imageRect18;
            imageRect19.Corner = new System.Windows.Forms.Padding(1);
            imageRect19.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect19.Image = global::SoftPhoneCSharp.Properties.Resources.key_hover;
            imageRect19.Source = new System.Drawing.Rectangle(0, 0, 52, 36);
            this.rdbVoiceSet.BackgroundHover = imageRect19;
            imageRect20.Corner = new System.Windows.Forms.Padding(0);
            imageRect20.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect20.Image = null;
            imageRect20.Source = new System.Drawing.Rectangle(0, 0, 0, 0);
            this.rdbVoiceSet.BackgroundNormal = imageRect20;
            imageRect21.Corner = new System.Windows.Forms.Padding(1);
            imageRect21.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect21.Image = global::SoftPhoneCSharp.Properties.Resources.key_pressed;
            imageRect21.Source = new System.Drawing.Rectangle(0, 0, 49, 31);
            this.rdbVoiceSet.BackgroundPushed = imageRect21;
            imageRect22.Corner = new System.Windows.Forms.Padding(1);
            imageRect22.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect22.Image = global::SoftPhoneCSharp.Properties.Resources.key_pressed;
            imageRect22.Source = new System.Drawing.Rectangle(0, 0, 49, 31);
            this.rdbVoiceSet.BackgroundSelected = imageRect22;
            this.rdbVoiceSet.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(204)))), ((int)(((byte)(204)))), ((int)(((byte)(204)))));
            this.rdbVoiceSet.Cursor = System.Windows.Forms.Cursors.Hand;
            this.rdbVoiceSet.DisableColor = System.Drawing.Color.DarkGray;
            this.rdbVoiceSet.Dock = System.Windows.Forms.DockStyle.Top;
            this.rdbVoiceSet.Image = global::SoftPhoneCSharp.Properties.Resources.setting_audio;
            this.rdbVoiceSet.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.rdbVoiceSet.ImagePadding = new System.Windows.Forms.Padding(10, 0, 0, 0);
            this.rdbVoiceSet.Location = new System.Drawing.Point(0, 96);
            this.rdbVoiceSet.Name = "rdbVoiceSet";
            this.rdbVoiceSet.ShowBorder = false;
            this.rdbVoiceSet.ShowBottomBorder = true;
            this.rdbVoiceSet.Size = new System.Drawing.Size(270, 48);
            this.rdbVoiceSet.TabIndex = 5;
            this.rdbVoiceSet.TabStop = true;
            this.rdbVoiceSet.Text = "音频设置";
            this.rdbVoiceSet.TextPadding = new System.Windows.Forms.Padding(50, 0, 0, 0);
            this.rdbVoiceSet.UseVisualStyleBackColor = false;
            this.rdbVoiceSet.CheckedChanged += new System.EventHandler(this.rdbVoiceSet_CheckedChanged);
            // 
            // rdbVideoSet
            // 
            this.rdbVideoSet.BackColor = System.Drawing.SystemColors.Control;
            imageRect23.Corner = new System.Windows.Forms.Padding(0);
            imageRect23.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect23.Image = null;
            imageRect23.Source = new System.Drawing.Rectangle(0, 0, 0, 0);
            this.rdbVideoSet.BackgroundDisabled = imageRect23;
            imageRect24.Corner = new System.Windows.Forms.Padding(1);
            imageRect24.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect24.Image = global::SoftPhoneCSharp.Properties.Resources.key_hover;
            imageRect24.Source = new System.Drawing.Rectangle(0, 0, 52, 36);
            this.rdbVideoSet.BackgroundHover = imageRect24;
            imageRect25.Corner = new System.Windows.Forms.Padding(0);
            imageRect25.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect25.Image = null;
            imageRect25.Source = new System.Drawing.Rectangle(0, 0, 0, 0);
            this.rdbVideoSet.BackgroundNormal = imageRect25;
            imageRect26.Corner = new System.Windows.Forms.Padding(1);
            imageRect26.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect26.Image = global::SoftPhoneCSharp.Properties.Resources.key_pressed;
            imageRect26.Source = new System.Drawing.Rectangle(0, 0, 49, 31);
            this.rdbVideoSet.BackgroundPushed = imageRect26;
            imageRect27.Corner = new System.Windows.Forms.Padding(1);
            imageRect27.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect27.Image = global::SoftPhoneCSharp.Properties.Resources.key_pressed;
            imageRect27.Source = new System.Drawing.Rectangle(0, 0, 49, 31);
            this.rdbVideoSet.BackgroundSelected = imageRect27;
            this.rdbVideoSet.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(204)))), ((int)(((byte)(204)))), ((int)(((byte)(204)))));
            this.rdbVideoSet.Cursor = System.Windows.Forms.Cursors.Hand;
            this.rdbVideoSet.DisableColor = System.Drawing.Color.DarkGray;
            this.rdbVideoSet.Dock = System.Windows.Forms.DockStyle.Top;
            this.rdbVideoSet.Image = global::SoftPhoneCSharp.Properties.Resources.setting_video;
            this.rdbVideoSet.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.rdbVideoSet.ImagePadding = new System.Windows.Forms.Padding(10, 0, 0, 0);
            this.rdbVideoSet.Location = new System.Drawing.Point(0, 48);
            this.rdbVideoSet.Name = "rdbVideoSet";
            this.rdbVideoSet.ShowBorder = false;
            this.rdbVideoSet.ShowBottomBorder = true;
            this.rdbVideoSet.Size = new System.Drawing.Size(270, 48);
            this.rdbVideoSet.TabIndex = 4;
            this.rdbVideoSet.TabStop = true;
            this.rdbVideoSet.Text = "视频设置";
            this.rdbVideoSet.TextPadding = new System.Windows.Forms.Padding(50, 0, 0, 0);
            this.rdbVideoSet.UseVisualStyleBackColor = false;
            this.rdbVideoSet.CheckedChanged += new System.EventHandler(this.rdbVideoSet_CheckedChanged);
            // 
            // rdbAccount
            // 
            this.rdbAccount.BackColor = System.Drawing.SystemColors.Control;
            imageRect28.Corner = new System.Windows.Forms.Padding(0);
            imageRect28.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect28.Image = null;
            imageRect28.Source = new System.Drawing.Rectangle(0, 0, 0, 0);
            this.rdbAccount.BackgroundDisabled = imageRect28;
            imageRect29.Corner = new System.Windows.Forms.Padding(1);
            imageRect29.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect29.Image = global::SoftPhoneCSharp.Properties.Resources.key_hover;
            imageRect29.Source = new System.Drawing.Rectangle(0, 0, 52, 36);
            this.rdbAccount.BackgroundHover = imageRect29;
            imageRect30.Corner = new System.Windows.Forms.Padding(0);
            imageRect30.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect30.Image = null;
            imageRect30.Source = new System.Drawing.Rectangle(0, 0, 0, 0);
            this.rdbAccount.BackgroundNormal = imageRect30;
            imageRect31.Corner = new System.Windows.Forms.Padding(1);
            imageRect31.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect31.Image = global::SoftPhoneCSharp.Properties.Resources.key_pressed;
            imageRect31.Source = new System.Drawing.Rectangle(0, 0, 49, 31);
            this.rdbAccount.BackgroundPushed = imageRect31;
            imageRect32.Corner = new System.Windows.Forms.Padding(1);
            imageRect32.Dest = new System.Drawing.Rectangle(0, 0, 0, 0);
            imageRect32.Image = global::SoftPhoneCSharp.Properties.Resources.key_pressed;
            imageRect32.Source = new System.Drawing.Rectangle(0, 0, 49, 31);
            this.rdbAccount.BackgroundSelected = imageRect32;
            this.rdbAccount.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(204)))), ((int)(((byte)(204)))), ((int)(((byte)(204)))));
            this.rdbAccount.Cursor = System.Windows.Forms.Cursors.Hand;
            this.rdbAccount.DisableColor = System.Drawing.Color.DarkGray;
            this.rdbAccount.Dock = System.Windows.Forms.DockStyle.Top;
            this.rdbAccount.Image = global::SoftPhoneCSharp.Properties.Resources.setting_account;
            this.rdbAccount.ImageAlign = System.Drawing.ContentAlignment.MiddleLeft;
            this.rdbAccount.ImagePadding = new System.Windows.Forms.Padding(10, 0, 0, 0);
            this.rdbAccount.Location = new System.Drawing.Point(0, 0);
            this.rdbAccount.Name = "rdbAccount";
            this.rdbAccount.ShowBorder = false;
            this.rdbAccount.ShowBottomBorder = true;
            this.rdbAccount.Size = new System.Drawing.Size(270, 48);
            this.rdbAccount.TabIndex = 3;
            this.rdbAccount.TabStop = true;
            this.rdbAccount.Text = "账号管理";
            this.rdbAccount.TextPadding = new System.Windows.Forms.Padding(50, 0, 0, 0);
            this.rdbAccount.UseVisualStyleBackColor = false;
            this.rdbAccount.CheckedChanged += new System.EventHandler(this.rdbAccount_CheckedChanged);
            // 
            // frmSetting
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(999, 628);
            this.Controls.Add(this.plFill);
            this.Controls.Add(this.plLeft);
            this.Name = "frmSetting";
            this.Text = "frmSetting";
            this.plLeft.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion
        private System.Windows.Forms.Panel plFill;
        private LightUI.PanelEx plLeft;
        private LightUI.RadioButtonEx rdbAccount;
        private LightUI.RadioButtonEx rdbVoiceSet;
        private LightUI.RadioButtonEx rdbVideoSet;
    }
}