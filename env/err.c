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

void myhandler1(MPI_Comm *comm,int *code,...)
{
   int me,len;
   char str[MPI_MAX_ERROR_STRING];

   MPI_Comm_rank(*comm,&me);
   if(me == 0)  {
      MPI_Error_string(*code,str,&len);
      printf("OOPS, ERROR NUMBER %d IN COMMUNICATOR %d\n",*code,*comm);
      printf("Message text is: %s\n",str);
      printf("\n");
   }
}

void myhandler2(MPI_Comm *comm,int *code,char *routine,int *flag,int *val)
{
   int me,len;
   char str[MPI_MAX_ERROR_STRING];

   MPI_Comm_rank(*comm,&me);
   if(me == 0)  {
      MPI_Error_string(*code,str,&len);
      printf("OOPS, ERROR NUMBER %d IN COMMUNICATOR %d\n",*code,*comm);
      printf("Message text is: %s\n",str);
      printf("Error occurred in %s\n",routine);
      if(flag)  printf("Offending value was: %d\n",*val);
      printf("\n");
   }
}

int main()
{
   int size,me,tasks,rc,class;
   MPI_Errhandler handler1,handler2,commhandler;
   MPI_Comm comm;

   MPI_Init(0,0);
   MPI_Comm_rank(MPI_COMM_WORLD,&me);
   MPI_Comm_size(MPI_COMM_WORLD,&tasks);

   if(tasks != 1)  { printf("MUST RUN WITH 1 TASK\n"); exit(0); }

      printf("THIS TESTCASE PRINTS 'OOPS' MESSAGES, DON'T BE SCARED\n\n\n");
      MPI_Errhandler_create(myhandler1,&handler1);
      MPI_Errhandler_create(myhandler2,&handler2);

      MPI_Comm_dup(MPI_COMM_WORLD,&comm);

      MPI_Errhandler_set(comm,handler1);   
      MPI_Errhandler_get(comm,&commhandler);
      if(commhandler != handler1) 
         printf("ERROR in MPI_Errhandler_get, handler = %d, should be %d\n",commhandler,handler1); 
      MPI_Bcast(&size,-1,-1,-1,comm); 

      MPI_Errhandler_set(comm,handler2);   
      MPI_Bcast(&size,1,-1,-1,comm); 

      commhandler = handler2;
      MPI_Errhandler_free(&handler2);
      if(handler2 != MPI_ERRHANDLER_NULL)
         printf("ERROR in MPI_Errorhandler_free, handle not set to NULL\n");
      MPI_Bcast(&size,1,-1,-1,comm); 

      MPI_Errhandler_set(comm,commhandler);   

      MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN);   
      rc = MPI_Errhandler_create(0,&handler2);
      MPI_Error_class(rc,&class);
      if(class != MPI_ERR_OTHER)  printf("ERROR: NULL function not detected\n");

   MPI_Barrier(MPI_COMM_WORLD);
   if(me == 0)  printf("ALL TESTS COMPLETE\n");
   MPI_Finalize();
}
