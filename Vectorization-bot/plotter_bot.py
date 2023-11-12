#TODO: add connective functionalities for transmission of vector data
#      dynamic command utilities
#      feedback utilities
#      more to come..

import logging
import numpy as np
import cairosvg as svg
from random import *
from linedraw import *
from io import BytesIO
from typing import Final
from telegram import Update, File, InlineKeyboardButton, InlineKeyboardMarkup
from telegram.ext import ContextTypes, Application, CommandHandler, MessageHandler, filters, ConversationHandler, CallbackQueryHandler


Username: Final = '@pen_plotter_bot'
Token: Final = '6617301227:AAGX9f5d5BvC66KXAy0_EKVXuuOltDIiJtQ' 
Ohearn = ['https://media.tenor.com/IX3oamddJ9cAAAAC/starwell-baby-don%27t-hurt-me.gif',
          'https://media.tenor.com/GUhnxCpzr78AAAAC/mike-ohearn-baby-dont-hurt-me.gif',
          'https://media.tenor.com/gfz3LnymsS4AAAAd/baby-dont-hurt-me-meme.gif',
          'https://media.tenor.com/aF9g0BotesYAAAAM/mike-o-heam-sigma-male.gif']

WAIT, UPLOAD, CONFIRMATION = range(3)

logging.basicConfig(
    format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level = logging.INFO
)

# Entry point: Starts conversation by simply greeting the user -> WAIT state
async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = f'Welcome {update.effective_user.full_name}!\nChoose a command!'
    )
    
    return WAIT

# Entry point: Restarts conversation from the start after bot shutdown -> WAIT state
async def restart(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Welcome back! The bot had shutdown but is now able to function again!' 
    )
    
    return WAIT

# Fallback (to change): Cancels the conversation -> WAIT state
async def cancel(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Canceling the operation..'
    )
    
    return WAIT

# WAIT component: what is love -> WAIT state
async def mike(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.sendDocument(
        chat_id = update.message.chat_id,
        document = Ohearn[randint(0, len(Ohearn) - 1)]
    )
    
    return WAIT

# WAIT component: Communicates that the system now waits for a picture -> UPLOAD state
async def upload(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Upload a picture you wish to have vectorized!'
    )
    
    return UPLOAD

# UPLOAD component: Receives unexpected message (not a picture) and awaits for an image -> UPLOAD state
async def reject(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Message is NOT a picture, awaiting for a new picture!'
    )
    
    return UPLOAD

# UPLOAD component - Core component: takes care of all the processing -> CONFIRMATION state
async def process(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Thank you! I will now start the image processing'
    )
    
    file = await context.bot.get_file(update.message.photo[-3].file_id)
    obj_file = await file.download_as_bytearray()
    
    image = cv2.imdecode(np.frombuffer(BytesIO(obj_file).read(), np.uint8), 1)
    cv2.imwrite('images/photo.png', image)
    
    file = None
    obj_file = None
    image = None
    
    await update.message.reply_text('Image received. Please hold tight for feedback while I lazily process the image.')
    
    await image_to_json('photo', draw_contours = 2, draw_hatch = 16, message = update.message)
    
    svg.svg2pdf(url = 'images/photo.svg', write_to = 'images/vectors.png')

    await update.message.reply_photo(photo = open('images/vectors.png', 'rb'))
    await update.message.reply_text(
        text = 'The picture has been fully processed. Do you wish to continue with the operation and forward the data?', 
        reply_markup = InlineKeyboardMarkup([[InlineKeyboardButton('Yes', callback_data = 'ul_confirmed'), InlineKeyboardButton('No', callback_data = 'ul_denied')]])
    )
    
    return CONFIRMATION

async def confirmation(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    query = update.callback_query
    await query.answer()
    
    match query.data:
        case 'ul_confirmed':
            await update.callback_query.message.reply_text(
                text = 'Uploading the data, you will be notified on the status of the printing process!\nNB: This is a work in progress'
            )
            await query.delete_message()
            return WAIT
        case 'ul_denied':
            await update.callback_query.message.reply_text(
                text = 'Do you wish to upload a new picture or cancel the operation?',
                reply_markup = InlineKeyboardMarkup([[InlineKeyboardButton('New picture', callback_data = 'new_pic'), InlineKeyboardButton('Cancel operation', callback_data = 'cancel_op')]])
            )
            await query.delete_message()
            return CONFIRMATION
        case 'new_pic':
            await update.callback_query.message.reply_text(
                text = 'Upload a picture you wish to have vectorized!'
            )
            await query.delete_message()
            return UPLOAD
        case 'cancel_op':
            await update.callback_query.message.reply_text(
                text = 'Canceling the operation..'
            )
            await query.delete_message()
            return WAIT
        case _:
            await update.callback_query.message.reply_text(
                text = 'There has been an issue with your request.'
            )
            await query.delete_message()
            return WAIT

if __name__ == "__main__":
    # Configures application
    app = Application.builder().token(Token).build()

    # Conversation Handler for application (bot)
    app.add_handler(ConversationHandler(       
        # Entry points for conversation
        # - /start command
        # - Any message after bot has shutdown         
        entry_points = [CommandHandler('start', start), MessageHandler(filters.ALL, restart)],
        # Conversation states, handle message transactions
        # - WAIT: awaits for user to wish to upload the image to vectorize
        # - UPLOAD: waits for picture to vectorize, rejects anything else
        # - CONFIRMATION: handles all user confirmation (picture is correct, vectorized image is to their liking, they want to start the drawing process)
        states = {
            WAIT: [
                MessageHandler(~filters.COMMAND, mike),
                CommandHandler('upload', upload)
            ],
            UPLOAD: [
                MessageHandler(~filters.PHOTO & (~filters.COMMAND), reject),
                MessageHandler(filters.PHOTO, process)
            ],
            CONFIRMATION: [
                CallbackQueryHandler(confirmation)
            ]
        },
        # Conversation fallbacks, conditions that make the conversation stop TODO: fix
        fallbacks = [CommandHandler('cancel', cancel)]
    ))
    
    # Starts application
    app.run_polling()
