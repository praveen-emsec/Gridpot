# gridpot-dev
Private gridpot repo

# Install 

First, install mysqld

Set up conpot with:

conpot/sudo make clean (if rebuilding)

conpot/sudo python setup.py install

Set up gridlabd with:

sudo make install

# Demo commands

We'll do a demo with the IEEE_13_Node_Test_Feeder.glm

for GridLAB-D 

cd gridlabd/3.1/models

gridlabd -D run_realtime=1 --server --debug --verbose IEEE_13_Node_Test_Feeder.glm 


For conpot:

sudo conpot -t gridpot

# Useful Developer Links

To expand the GridLAB-D objects we can interact with from conpot, one should see:
http://gridlab-d.sourceforge.net/wiki/index.php/Power_Flow_User_Guide
