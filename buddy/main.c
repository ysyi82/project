/********************************************************/
/*                                                      */
/* Project      : Buddy System                          */
/* File         : main.c                                */
/* Author       : Youn DaeSeok(woodsmano@gmail.com)     */
/* Company      : Dankook Univ. Embedded system lab.    */
/* Note         : buddy 테스트를 위한 main함수 		*/
/* Date         : 2008.7.3                              */
/*                                                      */
/********************************************************/

#include "Header/buddy.h"

int main( void )
{
	printf("[%x]\n", PAGE_SIZE << 4);
	printf("[%x]\n", 1UL << 16);
	printf("[%x]\n", ~3 );
	printf("[%x]\n", ((4096)+(sizeof(long))-1) );

	printf("[%x]\n", ((4096)+(sizeof(long))-1) & (~3));

	
	input_size();

	printf("[%x][%x]\n", PAGE_SIZE, PAGE_MASK);
	
	int i = 0;
	struct page* page1;	
	struct page* page2;	
	struct page* page3;	
	
	init_memory();

	printf("Initial State\n");
	for( i = 0; i <= 3; i++ )
		_show_free_order_list( i );

	printf( "\n\n" );

	printf("Order1 page allocation\n");
	page1 = alloc_pages(0, 1);
	for( i = 0; i <= 3; i++ )
		_show_free_order_list( i );

	printf( "\n\n" );

	printf("Order2 page allocation\n");
	page2 = alloc_pages(0, 2);
	for( i = 0; i <= 3; i++ )
		_show_free_order_list( i );

	printf( "\n\n" );
	
	printf("Order3 page allocation\n");
	page3 = alloc_pages(0, 3);
	for( i = 0; i <= 3; i++ )
		_show_free_order_list( i );

	_free_pages( page1->addr );
	_free_pages( page2->addr );
	_free_pages( page3->addr );

	free_memory();

	return 0;
}
