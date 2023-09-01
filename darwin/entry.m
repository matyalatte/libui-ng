// 14 august 2015
#import "uipriv_darwin.h"

// Text fields for entering text have no intrinsic width; we'll use the default Interface Builder width for them.
#define textfieldWidth 96

@interface uiprivNSTextField : NSTextField
@end

@implementation uiprivNSTextField

- (NSSize)intrinsicContentSize
{
	NSSize s;

	s = [super intrinsicContentSize];
	s.width = textfieldWidth;
	return s;
}

- (NSDragOperation)draggingEntered:(id)sender
{
	if ( [[[sender draggingPasteboard] types] containsObject:NSFilenamesPboardType] ) {
		return NSDragOperationCopy;
	}
	return NSDragOperationNone;
}

- (NSDragOperation)draggingUpdated:(id)sender
{
	if ( [[[sender draggingPasteboard] types] containsObject:NSFilenamesPboardType] ) {
		return NSDragOperationCopy;
	}
	return NSDragOperationNone;
}

- (BOOL)prepareForDragOperation:(id)sender
{
	return YES;
}

-(BOOL)performDragOperation:(id<NSDraggingInfo>)sender
{
	return YES;
}

-(void)concludeDragOperation:(id<NSDraggingInfo>)sender
{
	[self.delegate onFilesDropped:sender];
}

@end

// TODO does this have one on its own?
@interface uiprivNSSecureTextField : NSSecureTextField
@end

@implementation uiprivNSSecureTextField

- (NSSize)intrinsicContentSize
{
	NSSize s;

	s = [super intrinsicContentSize];
	s.width = textfieldWidth;
	return s;
}

- (NSDragOperation)draggingEntered:(id)sender
{
	if ( [[[sender draggingPasteboard] types] containsObject:NSFilenamesPboardType] ) {
		return NSDragOperationCopy;
	}
	return NSDragOperationNone;
}

- (NSDragOperation)draggingUpdated:(id)sender
{
	if ( [[[sender draggingPasteboard] types] containsObject:NSFilenamesPboardType] ) {
		return NSDragOperationCopy;
	}
	return NSDragOperationNone;
}

- (BOOL)prepareForDragOperation:(id)sender
{
	return YES;
}

-(BOOL)performDragOperation:(id<NSDraggingInfo>)sender
{
	return YES;
}

-(void)concludeDragOperation:(id<NSDraggingInfo>)sender
{
	[self.delegate onFilesDropped:sender];
}

@end

// TODO does this have one on its own?
@interface uiprivNSSearchField : NSSearchField
@end

@implementation uiprivNSSearchField

- (NSSize)intrinsicContentSize
{
	NSSize s;

	s = [super intrinsicContentSize];
	s.width = textfieldWidth;
	return s;
}

- (NSDragOperation)draggingEntered:(id)sender
{
	if ( [[[sender draggingPasteboard] types] containsObject:NSFilenamesPboardType] ) {
		return NSDragOperationCopy;
	}
	return NSDragOperationNone;
}

- (NSDragOperation)draggingUpdated:(id)sender
{
	if ( [[[sender draggingPasteboard] types] containsObject:NSFilenamesPboardType] ) {
		return NSDragOperationCopy;
	}
	return NSDragOperationNone;
}

- (BOOL)prepareForDragOperation:(id)sender
{
	return YES;
}

-(BOOL)performDragOperation:(id<NSDraggingInfo>)sender
{
	return YES;
}

-(void)concludeDragOperation:(id<NSDraggingInfo>)sender
{
	[self.delegate onFilesDropped:sender];
}

@end

struct uiEntry {
	uiDarwinControl c;
	NSTextField *textfield;
	void (*onChanged)(uiEntry *, void *);
	void *onChangedData;
	void (*onFilesDropped)(uiEntry *, int, char**, void *);
	void *onFilesDroppedData;
	int acceptDrops;
};

static BOOL isSearchField(NSTextField *tf)
{
	return [tf isKindOfClass:[NSSearchField class]];
}

@interface uiprivEntryDelegate : NSObject<NSTextFieldDelegate> {
	uiEntry *entry;
}
- (id)initWithEntry:(uiEntry *)e;
- (void)controlTextDidChange:(NSNotification *)notification;
- (IBAction)onChanged:(id)sender;
- (void)onFilesDropped:(id <NSDraggingInfo>)sender;
@end

@implementation uiprivEntryDelegate

- (id)initWithEntry:(uiEntry *)e
{
	self = [super init];
	if (self)
		self->entry = e;
	return self;
}

- (void)controlTextDidChange:(NSNotification *)notification
{
	[self onChanged:[notification object]];
}

- (IBAction)onChanged:(id)sender
{
	uiEntry *e = self->entry;;

	(*(e->onChanged))(e, e->onChangedData);
}

- (void)onFilesDropped:(id <NSDraggingInfo>)sender
{
	NSPasteboard *pboard = [sender draggingPasteboard];
	if ( ![[pboard types] containsObject:NSFilenamesPboardType] )
		return;
	NSArray *files = [pboard propertyListForType:NSFilenamesPboardType];
	int count = [files count];
	if (count == 0)
		return;

	char** names = (char**)uiprivAlloc((count + 1) * sizeof(char*), "char*[] names");
	char** start = names;
	for (int i = 0; i < count; i++) {
		NSString *nsfile = [files objectAtIndex:i];
		int size = [nsfile lengthOfBytesUsingEncoding:NSUTF8StringEncoding];
		*names = (char*)uiprivAlloc((size + 1) * sizeof(char), "char[] name");
		strcpy(*names, [nsfile UTF8String]);
		names++;
	}

	uiEntry *e = self->entry;
	(*(e->onFilesDropped))(e, count, start, e->onFilesDroppedData);

	names = start;
	for (int i = 0; i < count; i++) {
		uiprivFree(*names);
		names++;
	}

	uiprivFree(start);
}

@end

uiDarwinControlAllDefaultsExceptDestroy(uiEntry, textfield)

static void uiEntryDestroy(uiControl *c)
{
	uiEntry *e = uiEntry(c);
	uiprivEntryDelegate *delegate;

	if (isSearchField(e->textfield)) {
		delegate = [e->textfield target];
		[e->textfield setTarget:nil];
	} else {
		delegate = [e->textfield delegate];
		[e->textfield setDelegate:nil];
	}
	[delegate release];

	[e->textfield release];
	uiFreeControl(uiControl(e));
}

char *uiEntryText(uiEntry *e)
{
	return uiDarwinNSStringToText([e->textfield stringValue]);
}

void uiEntrySetText(uiEntry *e, const char *text)
{
	[e->textfield setStringValue:uiprivToNSString(text)];
	// don't queue the control for resize; entry sizes are independent of their contents
}

void uiEntryOnChanged(uiEntry *e, void (*f)(uiEntry *, void *), void *data)
{
	e->onChanged = f;
	e->onChangedData = data;
}

void uiEntryOnFilesDropped(uiEntry *e, void (*f)(uiEntry *, int, char**, void *), void *data)
{
	e->onFilesDropped = f;
	e->onFilesDroppedData = data;
}

int uiEntryReadOnly(uiEntry *e)
{
	return [e->textfield isEditable] == NO;
}

void uiEntrySetReadOnly(uiEntry *e, int readonly)
{
	BOOL editable;

	editable = YES;
	if (readonly)
		editable = NO;
	[e->textfield setEditable:editable];
}

int uiEntryAcceptDrops(uiEntry *e)
{
	return e->acceptDrops;
}

void uiEntrySetAcceptDrops(uiEntry *e, int accept)
{
	e->acceptDrops = accept != 0;
	if (accept)
		[e->textfield registerForDraggedTypes:[NSArray arrayWithObjects: NSFilenamesPboardType, nil]];
	else
		[e->textfield registerForDraggedTypes:nil];
}

char *uiEntryPlaceholder(uiEntry *e)
{
	return uiDarwinNSStringToText([e->textfield.cell placeholderString]);
}

void uiEntrySetPlaceholder(uiEntry *e, const char *text)
{
	[e->textfield.cell setPlaceholderString:uiprivToNSString(text)];
}

static void defaultOnChanged(uiEntry *e, void *data)
{
	// do nothing
}

static void defaultOnFilesDropped(uiEntry *e, int count, char** files, void *data)
{
	// do nothing
}

static NSTextField *realNewEditableTextField(Class class)
{
	NSTextField *tf;

	tf = [[class alloc] initWithFrame:NSZeroRect];
	uiprivNSTextFieldSetStyleEntry(tf);
	return tf;
}

NSTextField *uiprivNewEditableTextField(void)
{
	return realNewEditableTextField([uiprivNSTextField class]);
}

static uiEntry *finishNewEntry(Class class)
{
	uiEntry *e;
	uiprivEntryDelegate *delegate;

	uiDarwinNewControl(uiEntry, e);

	e->textfield = realNewEditableTextField(class);

	delegate = [[uiprivEntryDelegate alloc] initWithEntry:e];
	if (isSearchField(e->textfield)) {
		[e->textfield setTarget:delegate];
		[e->textfield setAction:@selector(onChanged:)];
	} else {
		[e->textfield setDelegate:delegate];
	}

	uiEntryOnChanged(e, defaultOnChanged, NULL);
	uiEntryOnFilesDropped(e, defaultOnFilesDropped, NULL);

	return e;
}

uiEntry *uiNewEntry(void)
{
	return finishNewEntry([uiprivNSTextField class]);
}

uiEntry *uiNewPasswordEntry(void)
{
	return finishNewEntry([uiprivNSSecureTextField class]);
}

uiEntry *uiNewSearchEntry(void)
{
	uiEntry *e;
	NSSearchField *s;

	e = finishNewEntry([uiprivNSSearchField class]);
	s = (NSSearchField *) (e->textfield);
	// TODO these are only on 10.10
//	[s setSendsSearchStringImmediately:NO];
//	[s setSendsWholeSearchString:NO];
	uiprivNSTextFieldSetStyleSearchEntry(s);
	return e;
}
