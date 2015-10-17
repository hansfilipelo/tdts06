import javax.swing.*;

public class RouterNode {
  private int myID;
  private GuiTextArea myGUI;
  private RouterSimulator sim;
  private int[] costs = new int[RouterSimulator.NUM_NODES];
  private int[] dVector = new int[RouterSimulator.NUM_NODES];
  private int[] routes = new int[RouterSimulator.NUM_NODES];
  private boolean enablePoison = true;

  //--------------------------------------------------
  public RouterNode(int ID, RouterSimulator sim, int[] costs) {
    myID = ID;
    this.sim = sim;
    myGUI =new GuiTextArea("  Output window for Router #"+ ID + "  ");

    System.arraycopy(costs, 0, this.costs, 0, RouterSimulator.NUM_NODES);
    System.arraycopy(costs, 0, this.dVector, 0, RouterSimulator.NUM_NODES);

    // Initiate routes for all neighboring nodes
    for (int i = 0; i < RouterSimulator.NUM_NODES ; i++ ) {
      if (costs[i] != RouterSimulator.INFINITY){
        routes[i] = i;
      }
      else {
        routes[i] = myID;
      }
    }

    // Send my costs to all neighboring nodes
    for (int i = 0; i < RouterSimulator.NUM_NODES ; i++ ) {
      if (costs[i] != RouterSimulator.INFINITY && i != myID){
        sendUpdate(new RouterPacket(myID,i,dVector));
      }
    }
  }

  //--------------------------------------------------
  public void recvUpdate(RouterPacket pkt) {
    boolean costChange = false;

    for (int i = 0; i < RouterSimulator.NUM_NODES ; i++ ) {
      if ( pkt.mincost[i] + costs[pkt.sourceid] < dVector[i] ){
        dVector[i] = pkt.mincost[i] + costs[pkt.sourceid];
        routes[i] = routes[pkt.sourceid];
        costChange = true; // If costs change - tell others
      }
      if ( routes[i] == pkt.sourceid && pkt.mincost[i] + costs[pkt.sourceid] > costs[i] ) {
        dVector[i] = costs[i];
        routes[i] = i;
        costChange = true;
      }
      if (!costChange && pkt.mincost[i] > dVector[i] + dVector[pkt.sourceid]){
        sendUpdate(new RouterPacket(myID,i,dVector));
      }
    }

    // If costs has changed - send update to all neigboring nodes
    if (costChange){
      for (int i = 0; i < RouterSimulator.NUM_NODES ; i++ ) {
        if (costs[i] != RouterSimulator.INFINITY && i != myID)
          sendUpdate(new RouterPacket(myID,i,dVector));
      }
    }
  }


  //--------------------------------------------------
  private void sendUpdate(RouterPacket pkt) {

    // Poison reverse
    if ( enablePoison ){
      for (int i = 0; i < RouterSimulator.NUM_NODES; i++ ) {
        if (routes[i] == pkt.destid){
          pkt.mincost[i] = RouterSimulator.INFINITY;
        }
      }
    }
    sim.toLayer2(pkt);
  }


  //--------------------------------------------------
  public void printDistanceTable() {
    myGUI.println("Current table for " + myID +
    "  at time " + sim.getClocktime());

    myGUI.println("Our distance vector and routes:");
    myGUI.println("--------------------------------");

    String destLine = "destination |  ";
    for (int i = 0; i < RouterSimulator.NUM_NODES ; i++) {
      destLine += i + " ";
    }
    myGUI.println(F.format(destLine,10));
    myGUI.println("--------------------------------");

    String costString = "cost | ";
    String routeString = "route | ";
    for (int i = 0; i < RouterSimulator.NUM_NODES ; i++ ) {
      costString += dVector[i] + "  ";
      routeString += routes[i] + "  ";
    }
    myGUI.println(F.format(costString,10));
    myGUI.println(F.format(routeString,10));
  }

  //--------------------------------------------------
  public void updateLinkCost(int dest, int newcost) {
    costs[dest] = newcost;
    boolean costChange = false;

    if (newcost < dVector[dest]){
      dVector[dest] = newcost;
      routes[dest] = dest;
      costChange = true;
    }
    else if (routes[dest] == dest){
      dVector[dest] = newcost;
      costChange = true;
    }

    // If costs has changed - send update to all neigboring nodes
    if (costChange){
      for (int i = 0; i < RouterSimulator.NUM_NODES ; i++ ) {
        if (costs[i] != RouterSimulator.INFINITY && i != myID)
        sendUpdate(new RouterPacket(myID,i,dVector));
      }
    }
  }

}
