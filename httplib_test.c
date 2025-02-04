#include "httplib.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void empty_callback() {}

void test_callback_1() {
    printf("Hello there\n");
}

void test_callback_2() {
    printf("Hello there again\n");
}

void test_uri_with_existing_node() {
    // Arrange
    destroy_tree(&root);
    setup();
    char route1[] = "GET /test/something/something_else";
    char route2[] = "GET /test/something";

    // Act
    register_route(route1, *test_callback_1);
    register_route(route2, *test_callback_2);

    // Assert
    assert(root.children->children->callback == &test_callback_2);
    assert(root.children->children->children->callback == &test_callback_1);
}

void test_should_properly_attach_to_children() {
    // Arrange
    destroy_tree(&root);
    setup();
    char parent[] = "GET /parent";
    char child[] = "GET /parent/child/otherchild";

    // Act
    register_route(parent, empty_callback);
    register_route(child, empty_callback);

    // Assert
    assert(!strcmp(root.children->val, "parent"));
    assert(!strcmp(root.children->children->val, "child"));
    assert(!strcmp(root.children->children->children->val, "otherchild"));
}

void test_should_properly_attach_to_children_with_siblings() {
    // Arrange
    destroy_tree(&root);
    setup();
    char route1[] = "GET /parent/onechild";
    char route2[] = "GET /parent/child/otherchild";

    // Act
    register_route(route1, empty_callback);
    register_route(route2, empty_callback);

    // Assert
    assert(!strcmp(root.children->val, "parent"));
    assert(!strcmp(root.children->children->val, "child"));
    assert(!strcmp(root.children->children->siblings->val, "onechild"));
    assert(!strcmp(root.children->children->children->val, "otherchild"));
}

void test_should_handle_duplicate_segments() {
    // Arrange
    destroy_tree(&root);
    setup();
    char route1[] = "GET /first/second";
    char route2[] = "GET /second/first";

    // Act
    register_route(route1, empty_callback);
    register_route(route2, empty_callback);

    // Assert
    assert(!strcmp(root.children->val, "second"));
    assert(!strcmp(root.children->children->val, "first"));
    assert(!strcmp(root.children->siblings->val, "first"));
    assert(!strcmp(root.children->siblings->children->val, "second"));
}

void test_trace_tree() {
    // Arrange
    destroy_tree(&root);
    setup();
    char route1[] = "GET /parent/onechild";
    char route2[] = "GET /parent/child/otherchild";

    // Act
    register_route(route1, empty_callback);
    register_route(route2, empty_callback);

    // Assert
    char uniqueRoute[] = "/a/unique/route";
    char partiallyUniqueRoute[] = "/parent/onechild/unique";
    char perfectMatch[] = "/parent/onechild";
    char perfectMatchAgain[] = "/parent/child/otherchild";
    assert(!strcmp(trace_tree(uniqueRoute)->val, "root"));
    assert(!strcmp(trace_tree(partiallyUniqueRoute)->val, "onechild"));
    assert(!strcmp(trace_tree(perfectMatch)->val, "onechild"));
    assert(!strcmp(trace_tree(perfectMatchAgain)->val, "otherchild"));
    assert(!strcmp(trace_tree_exact(perfectMatch)->val, "onechild"));
    assert(trace_tree_exact(uniqueRoute) == NULL);
}


int main() {
    test_uri_with_existing_node();
    test_should_properly_attach_to_children();
    test_should_properly_attach_to_children_with_siblings();
    test_should_handle_duplicate_segments();
    test_trace_tree();

    printf("Tests passed!\n");
    return -1;
}
