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
    destroy_tree(&get_root);
    setup();
    char route1[] = "/test/something/something_else";
    char route2[] = "/test/something";

    // Act
    get(route1, *test_callback_1);
    get(route2, *test_callback_2);

    // Assert
    assert(get_root.children->children->callback == &test_callback_2);
    assert(get_root.children->children->children->callback == &test_callback_1);
}

void test_should_properly_attach_to_children() {
    // Arrange
    destroy_tree(&get_root);
    setup();
    char parent[] = "/parent";
    char child[] = "/parent/child/otherchild";

    // Act
    get(parent, empty_callback);
    get(child, empty_callback);

    // Assert
    assert(!strcmp(get_root.children->val, "parent"));
    assert(!strcmp(get_root.children->children->val, "child"));
    assert(!strcmp(get_root.children->children->children->val, "otherchild"));
}

void test_should_properly_attach_to_children_with_siblings() {
    // Arrange
    destroy_tree(&get_root);
    setup();
    char route1[] = "/parent/onechild";
    char route2[] = "/parent/child/otherchild";

    // Act
    get(route1, empty_callback);
    get(route2, empty_callback);

    // Assert
    assert(!strcmp(get_root.children->val, "parent"));
    assert(!strcmp(get_root.children->children->val, "child"));
    assert(!strcmp(get_root.children->children->siblings->val, "onechild"));
    assert(!strcmp(get_root.children->children->children->val, "otherchild"));
}

void test_should_handle_duplicate_segments() {
    // Arrange
    destroy_tree(&get_root);
    setup();
    char route1[] = "/first/second";
    char route2[] = "/second/first";

    // Act
    get(route1, empty_callback);
    get(route2, empty_callback);

    // Assert
    assert(!strcmp(get_root.children->val, "second"));
    assert(!strcmp(get_root.children->children->val, "first"));
    assert(!strcmp(get_root.children->siblings->val, "first"));
    assert(!strcmp(get_root.children->siblings->children->val, "second"));
}

void test_trace_tree() {
    // Arrange
    destroy_tree(&get_root);
    setup();
    char route1[] = "/parent/onechild";
    char route2[] = "/parent/child/otherchild";

    // Act
    get(route1, empty_callback);
    get(route2, empty_callback);

    // Assert
    char uniqueRoute[] = "/a/unique/route";
    char partiallyUniqueRoute[] = "/parent/onechild/unique";
    char perfectMatch[] = "/parent/onechild";
    char perfectMatchAgain[] = "/parent/child/otherchild";
    assert(!strcmp(trace_tree(uniqueRoute, &get_root)->val, "root"));
    assert(!strcmp(trace_tree(partiallyUniqueRoute, &get_root)->val, "onechild"));
    assert(!strcmp(trace_tree(perfectMatch, &get_root)->val, "onechild"));
    assert(!strcmp(trace_tree(perfectMatchAgain, &get_root)->val, "otherchild"));
    assert(!strcmp(trace_tree_exact(perfectMatch, &get_root)->val, "onechild"));
    assert(trace_tree_exact(uniqueRoute, &get_root) == NULL);
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
