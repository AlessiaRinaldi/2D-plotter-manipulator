#TODO: add connective functionalities for transmission of vector data
#      dynamic command utilities
#      feedback utilities
#      overall concise subroutine with user friendly interfacement
#      linedraw feedback to telegram feedback
#      more to come..

import logging
import numpy as np
import aspose.words as aw
from random import *
from linedraw import *
from io import BytesIO
from typing import Final
from telegram import Update, File, InlineKeyboardButton, InlineKeyboardMarkup
from telegram.ext import ContextTypes, Application, CommandHandler, MessageHandler, filters, ConversationHandler, CallbackQueryHandler


Username: Final = '@pen_plotter_bot'
Token: Final = 'TOKEN' 
Ohearn = ['https://media.tenor.com/IX3oamddJ9cAAAAC/starwell-baby-don%27t-hurt-me.gif',
          'https://media.tenor.com/GUhnxCpzr78AAAAC/mike-ohearn-baby-dont-hurt-me.gif',
          'https://media.tenor.com/gfz3LnymsS4AAAAd/baby-dont-hurt-me-meme.gif',
          'https://media.tenor.com/aF9g0BotesYAAAAM/mike-o-heam-sigma-male.gif']

WAIT, UPLOAD, CONFIRMATION = range(3)

logging.basicConfig(
    format = '%(asctime)s - %(name)s - %(levelname)s - %(message)s',
    level = logging.INFO
)

async def start(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = f'Welcome {update.effective_user.full_name}!\nChoose a command!'
    )
    
    return WAIT

async def restart(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Welcome back! The bot had shutdown but is now able to function again!' 
    )
    
    return WAIT

async def cancel(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Canceling the operation..'
    )
    
    return WAIT

async def mike(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.sendDocument(
        chat_id = update.message.chat_id,
        document = Ohearn[randint(0, len(Ohearn) - 1)]
    )
    
    return WAIT

async def upload(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Upload a picture you wish to have vectorized!'
    )
    
    return UPLOAD

async def reject(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Message is NOT a picture, awaiting for a new picture!'
    )
    
    return UPLOAD

async def process(update: Update, context: ContextTypes.DEFAULT_TYPE) -> int:
    await context.bot.send_message(
        chat_id = update.effective_chat.id,
        text = 'Thank you! I will now start the image processing'
    )
    
    file = await context.bot.get_file(update.message.photo[-1].file_id)
    obj_file = await file.download_as_bytearray()
    
    image = cv2.imdecode(np.frombuffer(BytesIO(obj_file).read(), np.uint8), 1)
    cv2.imwrite('images/photo.png', image)
    cv2.imshow('Image', image)
    cv2.waitKey(2000)
    cv2.destroyAllWindows()
    
    file = None
    obj_file = None
    image = None
    
    await update.message.reply_text('Image received. Please hold tight for feedback while I lazily process the image.')
    
    image_to_json('photo', draw_contours = 2, draw_hatch = 16)
    
    img = aw.DocumentBuilder(aw.Document()).insert_image('images/photo.svg')
    img.image_data.save('images/vectors.png')
    
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
            await query.delete_message()
            await update.callback_query.message.reply_text(
                text = 'Uploading the data, you will be notified on the status of the printing process!\nNB: This is a work in progress'
            )
            return WAIT
        case 'ul_denied':
            await query.delete_message()
            await update.callback_query.message.reply_text(
                text = 'Do you wish to upload a new picture or cancel the operation?',
                reply_markup = InlineKeyboardMarkup([[InlineKeyboardButton('New picture', callback_data = 'new_pic'), InlineKeyboardButton('Cancel operation', callback_data = 'cancel_op')]])
            )
            return CONFIRMATION
        case 'new_pic':
            await query.delete_message()
            await update.callback_query.message.reply_text(
                text = 'Upload a picture you wish to have vectorized!'
            )
            return UPLOAD
        case 'cancel_op':
            await query.delete_message()
            await update.callback_query.message.reply_text(
                text = 'Canceling the operation..'
            )
            return WAIT
        case _:
            await query.delete_message()
            await update.callback_query.message.reply_text(
                text = 'There has been an issue with your request.'
            )
            return WAIT

if __name__ == "__main__":
    app = Application.builder().token(Token).build()
    
    app.add_handler(ConversationHandler(
        entry_points = [CommandHandler('start', start), MessageHandler(filters.TEXT, restart)],
        states = {
            WAIT: [
                MessageHandler(filters.TEXT & (~filters.COMMAND), mike),
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
        fallbacks = [CommandHandler('cancel', cancel)]
    ))
    
    app.run_polling()