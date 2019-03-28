/****************************************************************************

 MESSAGE PASSING INTERFACE TEST CASE SUITE

 Copyright IBM Corp. 1995

 IBM Corp. hereby grants a non-exclusive license to use, copy, modify, and
 distribute this software for any purpose and without fee provided that the
 above copyright notice and the following paragraphs appear in all copies.

 IBM Corp. makes no representation that the test cases comprising this
 suite are correct or are an accurate representation of any standard.

 In no event shall IBM be liable to any party for direct, indirect, special
 incidental, or consequential damage arising out of the use of this software
 even if IBM Corp. has been advised of the possibility of such damage.

 IBM CORP. SPECIFICALLY DISCLAIMS ANY WARRANTIES INCLUDING, BUT NOT LIMITED
 TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN "AS IS" BASIS AND IBM
 CORP. HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
 ENHANCEMENTS, OR MODIFICATIONS.

****************************************************************************

 These test cases reflect an interpretation of the MPI Standard.  They are
 are, in most cases, unit tests of specific MPI behaviors.  If a user of any
 test case from this set believes that the MPI Standard requires behavior
 different than that implied by the test case we would appreciate feedback.

 Comments may be sent to:
    Richard Treumann
    treumann@kgn.ibm.com

****************************************************************************
*/
#include "mpi.h"

typedef struct {
   int data;
   int flag;
} mydt;

void less(mydt *in,mydt *inout,int *count,int *dt)
{
   int i;

   if(*dt != MPI_2INT)  printf("ERROR in less: wrong data type\n");

   for(i=0;i<*count;i++)
      inout[i].flag = (in[i].data < inout[i].data) && in[i].flag && inout[i].flag;
}

main()
{
   int me,tasks,i,root,commute,rc,class,temp;
   mydt info,result;
   MPI_Op op;

   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);
   MPI_Comm_size(MPI_COMM_WORLD,&tasks);

   commute = 0;
   MPI_Op_create(less,commute,&op);

   info.data = me;
   info.flag = 1;

   root = 0;
   MPI_Reduce(&info,&result,1,MPI_2INT,op,root,MPI_COMM_WORLD);
   if(me == root)
      if(result.flag != 1)
         printf("ERROR in MPI_Reduce(1): result = %d, should be 1\n",result.flag);

   root = tasks/2;
   MPI_Reduce(&info,&result,1,MPI_2INT,op,root,MPI_COMM_WORLD);
   if(me == root)
      if(result.flag != 1)
         printf("ERROR in MPI_Reduce(2): result = %d, should be 1\n",result.flag);

   info.data = tasks - me;
   root = tasks-1;
   MPI_Reduce(&info,&result,1,MPI_2INT,op,root,MPI_COMM_WORLD);
   if(me == root)
      if(result.flag != 0)
         printf("ERROR in MPI_Reduce(3): result = %d, should be 0\n",result.flag);

   info.data = me;
   if(me == 0)  info.data = 1;
   MPI_Reduce(&info,&result,1,MPI_2INT,op,root,MPI_COMM_WORLD);
   if(me == root)
      if(result.flag != 0)
         printf("ERROR in MPI_Reduce(4): result = %d, should be 0\n",result.flag);

   temp = op;
   MPI_Op_free(&op);
   if(op != MPI_OP_NULL)
      printf("ERROR in MPI_Op_free: op not set to NULL\n");

   MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);
   rc = MPI_Reduce(&info,&result,1,MPI_2INT,op,root,MPI_COMM_WORLD);
   MPI_Error_class(rc,&class);
   if(class != MPI_ERR_OP)
      printf("ERROR in MPI_Op_free: no error on NULL op\n"); 

   op = temp;
   rc = MPI_Reduce(&info,&result,1,MPI_2INT,op,root,MPI_COMM_WORLD);
   MPI_Error_class(rc,&class);
   if(class != MPI_ERR_OP)
      printf("ERROR in MPI_Op_free: op not freed\n"); 

   op = MPI_OP_NULL;
   MPI_Op_create(less,commute,&op);
   if(op != temp)
      printf("ERROR in MPI_Op_create: op not reused\n");

   op = MPI_SUM;
   rc = MPI_Op_free(&op);
   MPI_Error_class(rc,&class);
   if(class != MPI_ERR_OP)
      printf("ERROR in MPI_Op_free: no error on free MPI_SUM\n"); 

   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  printf("TEST COMPLETE\n");
   MPI_Finalize();
}
