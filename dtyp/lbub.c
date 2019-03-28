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

main( argc, argv )
int argc;
char **argv;
{
    MPI_Datatype newtype,newtype2,newtype3,newtype4,newtype5;
    MPI_Datatype newtype6,newtype7,newtype8,newtype9;
    MPI_Datatype aot[3];
    int aob[3];   
    int error=0;
    int numtasks,me;
    MPI_Aint extent;
    MPI_Aint aod[3],lb,ub;

    MPI_Init( &argc, &argv );

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &me);

    if ((numtasks != 1) && (me!=0)) { 
 #ifdef DB_TALK
      printf("Testcase %s uses one task, extraneous task #%d exited.\n",argv[0],me);
 #endif
      MPI_Finalize(); 
      exit();
    }

    MPI_Type_contiguous(4,MPI_INT, &newtype); /* newtype == 4 int */
    MPI_Type_commit(&newtype);

    aot[0] = newtype; aod[0] = 0, aob[0] = 1;
    aot[1] = MPI_UB; aod[1] = 97, aob[1] = 1;
    MPI_Type_struct(2,aob,aod,aot,&newtype2);
    MPI_Type_commit(&newtype2);

    MPI_Type_extent(newtype2, &extent);
    MPI_Type_lb(newtype2, &lb);
    MPI_Type_ub(newtype2, &ub);
    if ((extent!=100) | (lb!=0) | (ub!=97)) {
      error++;
      printf("Should be: Extent = 100, lb = 0, ub = 97.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
    }
#ifdef DB_TALK
      printf("Talking:\n");
      printf("Should be: Extent = 100, lb = 0, ub = 97.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
#endif 

    aot[0] = newtype; aod[0] = 0, aob[0] = 1;
    aot[1] = MPI_CHAR; aod[1] = 97, aob[1] = 1;
    MPI_Type_struct(2,aob,aod,aot,&newtype3);
    MPI_Type_commit(&newtype3);

    MPI_Type_extent(newtype3, &extent);
    MPI_Type_lb(newtype3, &lb);
    MPI_Type_ub(newtype3, &ub);
    if ((extent!=100) | (lb!=0) | (ub!=98)) {
      error++;
      printf("Should be: Extent = 100, lb = 0, ub = 98.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
    }

#ifdef DB_TALK
      printf("Talking:\n");
      printf("Should be: Extent = 100, lb = 0, ub = 98.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
#endif

    aot[0] = newtype; aod[0] = 0, aob[0] = 1;
    aot[1] = MPI_LB; aod[1] = 3, aob[1] = 1;
    aot[2] = MPI_UB; aod[2] = 94, aob[2] = 1;
    MPI_Type_struct(3,aob,aod,aot,&newtype4);
    MPI_Type_commit(&newtype4);

    MPI_Type_extent(newtype4, &extent);
    MPI_Type_lb(newtype4, &lb);
    MPI_Type_ub(newtype4, &ub);
    if ((extent!=92) | (lb!=3) | (ub!=94)) {
      error++;
      printf("Should be: Extent = 92, lb = 3, ub = 94.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
    }
#ifdef DB_TALK
      printf("Talking:\n");
      printf("Should be: Extent = 92, lb = 3, ub = 94.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
#endif

    aot[0] = newtype; aod[0] = 0, aob[0] = 2;
    aot[1] = MPI_LB; aod[1] = -3, aob[1] = 1;
    aot[2] = MPI_UB; aod[2] = 96, aob[2] = 1;
    MPI_Type_struct(3,aob,aod,aot,&newtype5);
    MPI_Type_commit(&newtype5);

    MPI_Type_extent(newtype5, &extent);
    MPI_Type_lb(newtype5, &lb);
    MPI_Type_ub(newtype5, &ub);
    if ((extent!=100) | (lb!=-3) | (ub!=96)) {
      error++;
      printf("Should be: Extent = 100 lb = -3, ub = 96.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
    }
#ifdef DB_TALK
      printf("Talking:\n");
      printf("Should be: Extent = 100 lb = -3, ub = 96.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
#endif

    aot[0] = newtype; aod[0] = 2, aob[0] = 2;
    aot[1] = MPI_LB; aod[1] = -3, aob[1] = 1;
    aot[2] = MPI_UB; aod[2] = 86, aob[2] = 1;
    MPI_Type_struct(3,aob,aod,aot,&newtype6);
    MPI_Type_commit(&newtype6);

    MPI_Type_extent(newtype6, &extent);
    MPI_Type_lb(newtype6, &lb);
    MPI_Type_ub(newtype6, &ub);
    if ((extent!=92) | (lb!=-3) | (ub!=86)) {
      error++;
      printf("Should be: Extent = 92  lb = -3, ub = 86.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
    }
#ifdef DB_TALK
      printf("Talking:\n");
      printf("Should be: Extent = 92  lb = -3, ub = 86.\n");
      printf("Is:        Extent = %d, lb = %d, ub = %d.\n", extent, lb, ub);
#endif

    if (!error)
      printf("Upper bound/lower bound/extent test passed.\n");
    else
      printf("ERRORS in bounds/extent test.\n");
 
    MPI_Finalize();
} 
