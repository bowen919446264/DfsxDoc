//
//  XEditPanGestureRecognizer.m
//  XEdit
//
//  Created by DFSX on 2018/8/3.
//  Copyright © 2018年 Lskyme. All rights reserved.
//

#import "XEditPanGestureRecognizer.h"
#import <UIKit/UIGestureRecognizerSubclass.h>

@interface XEditPanGestureRecognizer ()

@property (assign, readwrite, nonatomic) XEditPanDirection direction;

@property (assign, nonatomic) BOOL drag;
@property (assign, nonatomic) int moveX;
@property (assign, nonatomic) int moveY;

@end

@implementation XEditPanGestureRecognizer

+ (instancetype)panGestureWithDirection:(XEditPanDirection)direction target:(id)target selector:(SEL)selector {
    return [[XEditPanGestureRecognizer alloc] initWithWithDirection:direction target:target selector:selector];
}

- (instancetype)initWithWithDirection:(XEditPanDirection)direction target:(id)target selector:(SEL)selector {
    self = [super initWithTarget:target action:selector];
    self.direction = direction;
    return self;
}

- (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
    [super touchesMoved:touches withEvent:event];
    if (self.state == UIGestureRecognizerStateFailed) return;
    CGPoint nowPoint = [[touches anyObject] locationInView:self.view];
    CGPoint prevPoint = [[touches anyObject] previousLocationInView:self.view];
    _moveX += prevPoint.x - nowPoint.x;
    _moveY += prevPoint.y - nowPoint.y;
    if (!_drag) {
        if (abs(_moveX) > 5) {
            if (_direction == XEditPanDirectionVertical) {
                self.state = UIGestureRecognizerStateFailed;
            } else {
                _drag = YES;
            }
        } else if (abs(_moveY) > 5) {
            if (_direction == XEditPanDirectionHorizontal) {
                self.state = UIGestureRecognizerStateFailed;
            } else {
                _drag = YES;
            }
        }
    }
}

- (void)reset {
    [super reset];
    _drag = NO;
    _moveX = 0;
    _moveY = 0;
}
@end
