# 3ds-study
A study project for learning how to develop apps and games for the Nintendo 3DS using C. The goal is to gain experience with 3DS development and eventually create full games. Once I’m confident, I will separate the game-specific projects into a different repository.

## SETUP:
It was a bit challenging to set up the development environment on my personal computer, but it worked well on all other devices I tried. If you need help, I followed [this tutorial](https://gbatemp.net/attachments/nintendo-3ds-homebrewing-getting-started-guide-2-1-pdf.482875/) to finish the installation on my computer, and everything worked well after that.

## BUILD:
If everything is set up correctly with your devkitPro installation, you can just type `make` in your terminal to build the project.

## TEST/RUN:
Once your project is built, you can transfer the output files to your 3DS via an SD card, FTP, or any method you prefer. 

To dev-test the files you can also:
1. Open the Homebrew Launcher on your 3DS.
2. Press "Y" to open the developer options.
3. Navigate to `devkitPro/tools/bin/` and launch `3dslink.exe`.
4. Select your built file as the argument for 3dslink.

This method provides a smoother testing experience compared to just using the SD card.
