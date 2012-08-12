//
//  SCSync.m
//  SlideChat
//
//  Created by Frederic Jacobs on 11/8/12.
//  Copyright (c) 2012 Movies.io Inc. All rights reserved.
//

#import "SCSync.h"

@implementation SCSync



+ (id)sharedManager {
    static SCSync *sharedMyManager = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedMyManager = [[self alloc] init];
    });
    return sharedMyManager;
}

- (void) setToken:(NSString*)_token{
    
    self.token = _token;
    
}
- (void) setUsername:(NSString*)_username{
    self.username = _username;
}

- (NSString*) username{
    
    return [NSString stringWithString:username];
}
- (NSString*) token{
    
    return [NSString stringWithString:token];
}

- (id)init {
    if (self = [super init]) {
        if ([[[NSUserDefaults standardUserDefaults]objectForKey:@"username"]isEqualToString:@""] || [[NSUserDefaults standardUserDefaults]objectForKey:@"username" ] == nil){
            
        }
        
        else{
            username = [[NSUserDefaults standardUserDefaults]objectForKey:@"username" ];
            token =[[NSUserDefaults standardUserDefaults]objectForKey:@"token"];
        }

    }
    return self;
}


- (void)dealloc {
    // Should never be called, but just here for clarity really.
}


@end
