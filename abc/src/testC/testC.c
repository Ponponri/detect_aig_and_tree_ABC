#include "base/main/main.h"
#include<string.h>

ABC_NAMESPACE_IMPL_START

int TestC_FirstFunction(Abc_Ntk_t * pNtk);
int TestC_Part1(Abc_Ntk_t * pNtk);
int TestC_Part2(Abc_Ntk_t * pNtk, int argc, char **argv);

int TestC_FirstFunctionAbc(Abc_Frame_t * pAbc){
    Abc_Ntk_t *pNtk;
    int result;
    pNtk = Abc_FrameReadNtk(pAbc);

    if(pNtk == NULL){
        Abc_Print(-1, "Test_FirstFunctionAbc: Getting the target network fails.\n");
        return 0;
    }

    result = TestC_FirstFunction(pNtk);

    return result;
}

int TestC_FirstFunction(Abc_Ntk_t * pNtk){
    if(!Abc_NtkIsStrash(pNtk)){
        Abc_Print(-1, "TestC_FirstFunction: This command is only application to strashed networks.\n");
        return 0;
    }

    Abc_Print(1, "The network %s has:\n", Abc_NtkName(pNtk));
    Abc_Print(1, "\t- %d primary inputs;\n", Abc_NtkPiNum(pNtk));
    Abc_Print(1, "\t- %d primary outputs;\n", Abc_NtkPoNum(pNtk));
    Abc_Print(1, "\t- %d nodes;\n", Abc_NtkNodeNum(pNtk));
    return 1;

}

// part 1
int TestC_Part1Abc(Abc_Frame_t * pAbc){
    Abc_Ntk_t *pNtk;
    int result;
    pNtk = Abc_FrameReadNtk(pAbc);

    if(pNtk == NULL){
        Abc_Print(-1, "Test_FirstFunctionAbc: Getting the target network fails.\n");
        return 0;
    }

    result = TestC_Part1(pNtk);

    return result;
}

int TestC_Part1(Abc_Ntk_t * pNtk){
    if(!Abc_NtkIsStrash(pNtk)){
        Abc_Print(-1, "TestC_FirstFunction: This command is only application to strashed networks.\n");
        return 0;
    }
    Abc_Obj_t * current_obj;
    Abc_Obj_t * pre_obj;
    // Abc_Obj_t * post_obj;
    Abc_Print(1, "<< Print Each Obj - >>\n");
    Abc_Print(1, "ID\tName\tType\tLevel\n");
    Abc_Print(1, "---------------------\n");

    // Abc_Print(1, "The network %s has:\n", Abc_NtkName(pNtk));
    // Abc_Print(1, "\t- %d Spec;\n", Abc_NtkSpec(pNtk));
    // Abc_Print(1, "\t- %d Abc_NtkObjNum;\n", Abc_NtkObjNum(pNtk));
    for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
        current_obj = Abc_NtkObj(pNtk,i);
        // Abc_Print(1, "\t------------\n");
        Abc_Print(1, "\nId:\t%d,\t\t", Abc_ObjId(current_obj));
        Abc_Print(1, "Name\t%s,\t\t", Abc_ObjName(current_obj));
        Abc_Print(1, "Type\t%d,\t\t", Abc_ObjType(current_obj));
        Abc_Print(1, "Level\t%d,", Abc_ObjLevel(current_obj));



        // Abc_Print(1, "\tFanin:\n");
        for(int j=0;j<Abc_ObjFaninNum(current_obj);j++){
            pre_obj = Abc_ObjFanin(current_obj,j);
            // Abc_Print(1, "\t- %d Abc_ObjId;\n", Abc_ObjId(pre_obj));
            Abc_Print(1, "\tFiName\t%s, ", Abc_ObjName(pre_obj));
            // current_obj->pNtk->vPhases;
            // Abc_Print(1, "\tFiPhase\t%d, ", Abc_ObjFaninPhase(pre_obj,j));
            if(j==0)
                Abc_Print(1, "\tFiPhase\t%x, ", current_obj->fCompl0);
            if(j==1)
                Abc_Print(1, "\tFiPhase\t%x, ", current_obj->fCompl1);
        }

    }
    
    Abc_Print(1, "\n<< ------END------ >>\n");

    return 1;

}

// part 2

void dfs_root(Abc_Obj_t * pObj, int* isRoot);
void dfs_tree(Abc_Ntk_t * pNtk, int* isTree, int id);

int TestC_Part2Abc(Abc_Frame_t * pAbc, int argc, char **argv){
    Abc_Ntk_t *pNtk;
    int result;
    pNtk = Abc_FrameReadNtk(pAbc);

    if(pNtk == NULL){
        Abc_Print(-1, "Test_FirstFunctionAbc: Getting the target network fails.\n");
        return 0;
    }

    result = TestC_Part2(pNtk,argc,argv);

    return result;
}

int TestC_Part2(Abc_Ntk_t * pNtk, int argc, char **argv){
    if(!Abc_NtkIsStrash(pNtk)){
        Abc_Print(-1, "TestC_FirstFunction: This command is only application to strashed networks.\n");
        return 0;
    }

    Abc_Print(1, "\n<< ------ANDTREE------ >>\n");
    int num;
    if(argc>=3 && strcmp(argv[1],"-S")==0){
        num = atoi(argv[2]);
    }
    else{
        num = 5;
    }

    int isRoot[Abc_NtkObjNum(pNtk)];

    for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
        isRoot[i] = 0;
    }
    Abc_Obj_t * current_obj;
    Abc_Obj_t * pre_obj;

    for(int i=0;i<Abc_NtkPoNum(pNtk);i++){
        current_obj = Abc_NtkPo(pNtk,i);
        dfs_root(current_obj, isRoot);
        isRoot[Abc_ObjId(current_obj)] = 0;
        for(int i=0;i<Abc_ObjFaninNum(current_obj);i++){
            pre_obj = Abc_ObjFanin(current_obj,i);
            isRoot[Abc_ObjId(pre_obj)] = 1;
        }
    }
    for(int i=0;i<Abc_NtkPiNum(pNtk);i++){
        current_obj = Abc_NtkPi(pNtk,i);
        isRoot[Abc_ObjId(current_obj)] = 0;
    }
    // for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
    //     Abc_Print(1, "Id_%d: %d\n",i,isRoot[i]);
    // }
    
    int isTree[Abc_NtkObjNum(pNtk)];
    int isLeave[Abc_NtkObjNum(pNtk)];
    int count0,count1;
    for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
        if(isRoot[i] == 1){
            for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
                isTree[i] = 0;
            }
            dfs_tree(pNtk,isTree,i);
            // Abc_Print(1,"\n----------------\n");
            // Abc_Print(1,"Id:%d\n",i);
            // for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
            //     Abc_Print(1, "%d\t",isTree[i]);
            // }
            // Abc_Print(1, "\n");
            count0 = 0;
            count1 = 0;
            for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
                isLeave[i] = 0;
            }
            for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
                if(isTree[i] == 1){
                    count0+=1;
                    isLeave[i] = 1;
                    current_obj = Abc_NtkObj(pNtk,i);
                    for(int j=0;j<Abc_ObjFaninNum(current_obj);j++){
                        if(j==0){
                            pre_obj = Abc_ObjFanin0(current_obj);
                            isLeave[Abc_ObjId(pre_obj)] = 1;
                        }
                        else if(j==1){
                            pre_obj = Abc_ObjFanin1(current_obj);
                            isLeave[Abc_ObjId(pre_obj)] = 1;
                        }
                        
                    }
                }
            }
            for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
                if(isLeave[i] == 1){
                    count1+=1;
                }
            }

            if(count1-count0 >= num && count1-count0>1){
                Abc_Print(1,"\n%d",count1-count0);
                // Abc_Print(1,"\tc0:%d, c1:%d",count0,count1);
                for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
                    if(isTree[i] == 1){
                        Abc_Print(1,"\t%s",Abc_ObjName(Abc_NtkObj(pNtk,i)));
                    }
                }
            }
            

        }
    }
    Abc_Print(1, "\n<< ------END------ >>\n");
    return 1;

}

void dfs_tree(Abc_Ntk_t * pNtk, int* isTree, int id){
    int flag0;
    int cnt;
    Abc_Obj_t * current_obj;
    Abc_Obj_t * pre_obj;
    current_obj = Abc_NtkObj(pNtk,id);
    // Abc_Print(1, "\nId:%d",Abc_ObjId(current_obj));
    flag0 = 1;
    for(int i=0;i<Abc_ObjFaninNum(current_obj);i++){
        pre_obj = Abc_ObjFanin(current_obj,i);
        // Abc_Print(1, "\tfanin:%d",Abc_ObjId(pre_obj));
        cnt=0;
        for(int i=0;i<Abc_NtkObjNum(pNtk);i++){
            cnt+=isTree[i];
        }
        if(Abc_ObjFanoutNum(current_obj)>1 && cnt>0){
            flag0 = 0;
        }
        if(i==0){
            if(current_obj->fCompl0 == 1)
                flag0 = 0;
        }
        else if(i==1){
            if(current_obj->fCompl1 == 1)
                flag0 = 0;
        }
    }
    if(flag0 == 1 && Abc_ObjFaninNum(current_obj)>0){
        isTree[id] = 1;
        // Abc_Print(1, "\tadd");
        for(int i=0;i<Abc_ObjFaninNum(current_obj);i++){
            pre_obj = Abc_ObjFanin(current_obj,i);
            if((i==0 && current_obj->fCompl0 == 0) || (i==1 && current_obj->fCompl1 == 0)){
                dfs_tree(pNtk,isTree,Abc_ObjId(pre_obj));
            }

        }
        
    }
    
    
}

void dfs_root(Abc_Obj_t * pObj, int* isRoot){
    Abc_Obj_t * pre_obj;
    Abc_Obj_t * pre_obj0;
    Abc_Obj_t * pre_obj1;
    for(int i=0;i<Abc_ObjFaninNum(pObj);i++){
        pre_obj = Abc_ObjFanin(pObj,i);
        if(Abc_ObjFanoutNum(pObj)!=1){
            isRoot[Abc_ObjId(pObj)] = 1;
        }
        if(i==0){
            isRoot[Abc_ObjId(pre_obj)] = pObj->fCompl0;
            pre_obj0 = Abc_ObjFanin(pObj,i);
        }
        if(i==1){
            isRoot[Abc_ObjId(pre_obj)] = pObj->fCompl1;
            pre_obj1 = Abc_ObjFanin(pObj,i);
            if( pObj->fCompl0 == 1 && pObj->fCompl1 == 0){
                isRoot[Abc_ObjId(pre_obj1)] = 1;
            }
            if( pObj->fCompl0 == 0 && pObj->fCompl1 == 1){
                isRoot[Abc_ObjId(pre_obj0)] = 1;
            }
        }
        dfs_root(pre_obj,isRoot);
    }
}


ABC_NAMESPACE_IMPL_END