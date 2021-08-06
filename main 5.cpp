//
//  main.cpp
//  Project
//
//  Created by Conghui Deng on 3/30/20.
//  Copyright © 2020 Conghui Deng. All rights reserved.
//

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;

class Kmean{
public:
    class Point{
    public:
        double Xcoord;
        double Ycoord;
        int Label;
        double Distance;
//        Point(){
//            Label=0;
//            Distance=99999.00;
//        }
    };
    
    
    int K,numPts,numRows,numCols,change;
    int *check;
    Point *pointSet;
    Point *KcentroidAry;
     int **displayAry;
    void setPointSet(){
        pointSet=new Point[numPts];
        KcentroidAry=new Point[K+1];
        for(int i=1;i<K+1;i++){
            KcentroidAry[i].Label=i;
            KcentroidAry[i].Distance=0.0;
        }
        check=new int[K];
        for(int i=0;i<K+1;i++){
            check[i]=-1;
        }
//        setPointLabel();
    }
    
//    void setPointLabel(){
//         srand( static_cast<unsigned int>(time(nullptr)));
//        for(int i=0;i<numPts;i++){
//            pointSet[i].Label=rand()%K+1;
//
//        }
//
//    }
    
    void setDisplayAry(){
        displayAry= new int* [numRows];
        for(int i=0;i<numRows;i++){
            displayAry[i]=new int[numCols];
        }
        for(int i=0;i<numRows;i++)
            for(int j=0;j<numCols;j++)
                displayAry[i][j]=0;
    }
    
    void loadPointSet(ifstream &in){
        int index=0;
        double x,y;
        while(!in.eof()){
            in>>x>>y;
            pointSet[index].Xcoord=x;
            pointSet[index].Ycoord=y;
            //pointSet[index].Label=0;
            pointSet[index].Distance=99999.00;
            index++;
            
        }
        
    }
    bool checkRepeat(int index){
        for(int i=1;i<K+1;i++){
           if(check[i]==index)
               return true;
        }
        
        return false;
    }
    
    void kMeanClustering(ofstream &out){
        change=4;
        int iteration=0,index,minLabel;
        Point *pt=new Point;
        double minDist;
        selectKcentroids();
        while(change>2){
            change=0;
            index=0;
        
            while(index<numPts){
                pt=&pointSet[index];
                minDist=pt->Distance;
                minLabel=DistanceMinLabel(*pt,minDist);
                if(pointSet[index].Label!=minLabel){
                    pointSet[index].Label=minLabel;
                    //pointSet[index].Distance=minDist;
                    change++;
                }
                index++;
                
            }
            iteration++;
            PlotDisplayAry();
            prettyPrint(out,iteration);
            if(change>2){
                computeCentroids();
                
            }
        }
        
    }
    
    void printResult(ofstream &out){
        out<<numRows<<" "<<numCols<<endl;
        out<<numPts<<endl;
        for(int i=0;i<numRows;i++)
            for(int j=0; j<numCols; j++){
                if(displayAry[i][j]>0){
                    out<<i<<" "<<j<<" "<<displayAry[i][j]<<endl;
                }
            }
    }
    
    void computeCentroids(){
        double sumX[K+1],sumY[K+1];
        int totalPt[K+1],label;
        for(int i=0;i<K+1;i++){
            sumX[i]=0.0;
            sumY[i]=0.0;
            totalPt[i]=0;
        }
        int index=0;
        while(index<numPts){
            label=pointSet[index].Label;
            sumX[label] +=pointSet[index].Xcoord;
            sumY[label] +=pointSet[index].Ycoord;
            totalPt[label]++;
            index++;
        }
        label=1;
        for(int i=label;i<=K;i++){
            if(totalPt[label]>0.0){
                KcentroidAry[label].Xcoord=(sumX[label]/totalPt[label]);
                KcentroidAry[label].Ycoord=(sumY[label]/totalPt[label]);
            }
        }
    }
    
    int DistanceMinLabel(Point pt,double minD){
        minD=99999.00;
        int minLabel=0;
        int label=1;
        while(label<=K){
        Point *whichCentroid=&KcentroidAry[label];
        double dist=computeDist(pt,*whichCentroid);
        if(dist<minD){
            minD=dist;
            minLabel=label;
            pt.Distance=minD;
        }
        label++;
        }
        return minLabel;
    }
    
    
    double computeDist(Point pt,Point whichCentroid){
        double dist;
        double x1=pt.Xcoord,x2=whichCentroid.Xcoord;
        double y1=pt.Ycoord,y2=whichCentroid.Ycoord;
        dist=sqrt(pow(x1-x2,2)+pow(y1-y2,2));
        return dist;
    }
    
    void PlotDisplayAry(){
        for(int i=0;i<numPts;i++){
            int x=(int)pointSet[i].Xcoord;
            int y=(int)pointSet[i].Ycoord;
            displayAry[x][y]=pointSet[i].Label;
        }
        
    }
    
    void prettyPrint(ofstream &out,int iteration){
        out<<"Cluster K is: "<<K<<"     Iteration#: "<<iteration<<endl;
        for(int i=0;i<numRows;i++){
            for(int j=0;j<numCols;j++){
                if(displayAry[i][j]>0)
                    out<<displayAry[i][j];
                else
                    out<<" ";
            }
            out<<endl;
        }
        
    }
    
    void selectKcentroids(){
        int Kcnt=0,index=0;
        bool repeatYN=true;
         srand( static_cast<unsigned int>(time(nullptr)));
        while(Kcnt<K){
           while(repeatYN){
              index=rand()% numPts;
              repeatYN=checkRepeat(index);
           }
            Kcnt++;
            check[Kcnt]=index;
            KcentroidAry[Kcnt].Xcoord=pointSet[index].Xcoord;
            KcentroidAry[Kcnt].Ycoord=pointSet[index].Ycoord;
            KcentroidAry[Kcnt].Label=Kcnt;
            KcentroidAry[Kcnt].Distance=0.0;
           
            repeatYN=true;
    }
        
    }
};

int main(int argc,char *argv[]){
    ifstream inFile(argv[1]);
    int k=0;
    k=stoi(argv[2]);
    ofstream outFile1(argv[3]);
    ofstream outFile2(argv[4]);
    Kmean *km=new Kmean;
    inFile>>km->numRows>>km->numCols>>km->numPts;
    km->K=k;
    
    km->setDisplayAry();
    km->setPointSet();
    km->loadPointSet(inFile);
    km->kMeanClustering(outFile1);
    km->printResult(outFile2);
    inFile.close();
    outFile1.close();
    outFile2.close();
}
